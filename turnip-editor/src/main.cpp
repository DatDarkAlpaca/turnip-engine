#include "pch.hpp"
#include <turnip_engine.hpp>
#include <platform/platform.hpp>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
using namespace tur;

#include "widget/scene_data.hpp"
#include "widget/scene_viewer.hpp"
#include "widget/entity_inspector.hpp"
#include "widget/scene_editor.hpp"

// View:
struct MainView : public View
{
public:
	MainView()
	{
	}

public:
	void on_engine_startup() override
	{
		initialize_textures();
		
		initialize_renderer_system();

		create_scene();

		// Widgets:
		m_EntityInspector.initialize(&m_Scene, &m_SceneData);
		m_SceneViewer.initialize(&m_Scene, &m_SceneData);
		m_SceneEditor.initialize(&r_Engine->get_graphics_device(), &r_Engine->get_window(), &m_SceneData);
	}

	void on_update() override
	{
		m_Scene.on_update_runtime();
	}

	void on_render_gui() override
	{
		ImGui::DockSpaceOverViewport();

		ImGui::Begin("Runtime");
		if (ImGui::Button("Start"))
		{
			m_Scene.start_runtime();
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			m_Scene.stop_runtime();
		}
		ImGui::End();

		m_SceneEditor.on_render_gui();
		m_SceneViewer.on_render_gui();
		m_EntityInspector.on_render_gui();
	}

	void on_event(Event& event) override
	{
		Subscriber subscriber(event);
		subscriber.subscribe<WindowResizeEvent>([&](const WindowResizeEvent& event) -> bool {
			m_RenderSystem.get_renderer().set_viewport({ 0.f, 0.f, (float)event.width, (float)event.height });
			m_MainCamera.set_orthogonal(0.f, (float)event.width, (float)event.height, 0.f, -1.f, 1.f);
						
			return false;
		});

		m_RenderSystem.get_renderer().on_event(event);
	}

	void on_render() override
	{
		m_RenderSystem.render();
		// m_QuadRenderer.render();
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

		// Scene Texture:
		{
			TextureDescriptor descriptor;
			descriptor.width  = 640;
			descriptor.height = 480;

			m_SceneData.sceneTexture = r_Engine->get_graphics_device().create_texture(descriptor);
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
		auto windowSize = r_Engine->get_window().data.properties.dimensions;

		m_MainCamera.set_orthogonal(0.0f, (float)windowSize.x, 0.f, (float)windowSize.y, -1.f, 1.f);

		// Main:
		m_RenderSystem.initialize(r_Engine->get_config_data(), &r_Engine->get_graphics_device(), &m_MainCamera, &m_Scene);
		m_RenderSystem.get_renderer().set_clear_color({ 40.f / 255.f, 40.f / 255.f, 40.f / 255.f, 1.0f });
		m_RenderSystem.get_renderer().set_viewport({ 0.f, 0.f, (float)windowSize.x, (float)windowSize.y });
		m_RenderSystem.get_renderer().set_render_target_texture(m_SceneData.sceneTexture);

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
		float size = 50.f;

		m_Entity = m_Scene.add_entity();
		{
			glm::vec3 position = glm::vec3(640.f / 2.f, 480.f / 2.f, 0.f);
			glm::vec3 rotation = glm::vec3(0.f);
			glm::vec3 scale = glm::vec3(size, size, 1.f);

			m_Entity.add_component<TransformComponent>(position, rotation, scale);
		}

		auto e0 = m_Scene.add_entity();
		glm::vec3 position = glm::vec3(640.f / 2.f + size, 480.f / 2.f, 0.f);
		glm::vec3 scale = glm::vec3(size, size, 1.f);
		glm::vec3 rotation = glm::vec3(0.f);
		e0.add_component<TransformComponent>(position, rotation, scale);
	}

private:
	Scene m_Scene;
	Entity m_Entity;
	Camera m_MainCamera;
	QuadRendererSystem m_RenderSystem;
	InstancedQuadRenderer m_QuadRenderer;

private:
	SceneData m_SceneData;
	SceneViewer m_SceneViewer;
	SceneEditor m_SceneEditor;
	EntityInspector m_EntityInspector;

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
