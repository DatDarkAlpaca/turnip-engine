#include "pch.hpp"
#include <turnip_engine.hpp>
#include <platform/platform.hpp>

#include <imgui.h>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
using namespace tur;

// View:
struct MainView : public View
{
public:
	void on_engine_startup() override
	{
		initialize_textures();
		
		initialize_renderer_system();

		create_scene();
	}

	void on_render_gui() override
	{
		ImGui::Begin("Hello World");
		ImGui::End();
	}

	void on_event(Event& event) override
	{
		Subscriber subscriber(event);
		subscriber.subscribe<WindowResizeEvent>([&](const WindowResizeEvent& event) -> bool {
			m_RenderSystem.get_renderer().set_viewport({ 0.f, 0.f, (float)event.width, (float)event.height });
			m_MainCamera.set_orthogonal(0.f, (float)event.width, (float)event.height, 0.f, -1.f, 1.f);

			return false;
		});
	}

	void on_render() override
	{
		//m_RenderSystem.render();
		m_QuadRenderer.render();
	};

private:
	void initialize_textures()
	{
		using namespace std::chrono_literals;

		auto& assetLibrary = r_Engine->get_asset_library();
		
		// Default Texture:
		{
			auto [descriptor, asset] = create_default_texture();
			auto defaultTexture = r_Engine->get_graphics_device().create_texture(descriptor, asset);
			m_RenderSystem.get_renderer().set_default_texture(defaultTexture);
		}

		// Face:
		r_Engine->get_worker_pool().submit<asset_handle>([&]() {
			std::this_thread::sleep_for(1000ms);
			return load_texture_asset(&assetLibrary, "res/textures/face.png");
		}, [&](asset_handle handle) {
			auto texture = assetLibrary.textures.get(handle);
			TUR_LOG_INFO("Loaded texture successfully: {}", texture.filepath.string());

			TextureDescriptor descriptor;
			{
				descriptor.format = TextureFormat::RGBA8_UNORM;
				descriptor.type = TextureType::TEXTURE_2D;
				descriptor.width = texture.width;
				descriptor.height = texture.height;
				descriptor.generateMipmaps = true;
			}

			m_Texture = r_Engine->get_graphics_device().create_texture(descriptor, texture);
			m_Entity.add_component<TextureComponent>(m_Texture);

			m_QuadRenderer.add_texture(texture);
		});
	}

	void initialize_renderer_system()
	{
		auto windowSize = r_Engine->get_window().properties.dimensions;

		m_MainCamera.set_orthogonal(0.0f, (float)windowSize.x, 0.f, (float)windowSize.y, -1.f, 1.f);

		// Main:
		m_RenderSystem.initialize(r_Engine->get_config_data(), &r_Engine->get_graphics_device(), &m_MainCamera, &m_Scene);
		m_RenderSystem.get_renderer().set_clear_color({ 40.f / 255.f, 40.f / 255.f, 40.f / 255.f, 1.0f });
		m_RenderSystem.get_renderer().set_viewport({ 0.f, 0.f, (float)windowSize.x, (float)windowSize.y });

		// Instanced:
		m_QuadRenderer.initialize(r_Engine->get_config_data(), &r_Engine->get_graphics_device(), &m_MainCamera);
		m_QuadRenderer.set_clear_color({ 1.0f, 0.0f, 0.0f, 1.0f });
		m_QuadRenderer.set_viewport({ 0.f, 0.f, (float)windowSize.x, (float)windowSize.y });

		float size = 25.0;
		for (int x = 0; x < 10; ++x)
		{
			for (int y = 0; y < 10; ++y)
				m_QuadRenderer.add_quad({ { x * size + size, y * size, 0.0f }, { size, size }, 0 });
		}
	}

	void create_scene()
	{
		m_Entity = m_Scene.add_entity();
		
		glm::mat4 model(1.f);
		float scale = 50.f;
		model = glm::translate(model, glm::vec3(140.f / 2, 480.f / 2, 0.f));
		model = glm::scale(model, glm::vec3(scale, scale, 1.f));
		
		m_Entity.add_component<TransformComponent>(model);	
	}

private:
	Scene m_Scene;
	Entity m_Entity;
	Camera m_MainCamera;
	QuadRendererSystem m_RenderSystem;
	InstancedQuadRenderer m_QuadRenderer;

private:
	texture_handle m_Texture = invalid_handle;
};

int main()
{
	TurnipEngine engine;
	engine.initialize("engine_config.json");

	engine.add_view(tur::make_unique<MainView>());

	engine.run();

	return 0;
}
