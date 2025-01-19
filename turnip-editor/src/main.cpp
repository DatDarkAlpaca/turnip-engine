#include "pch.hpp"
#include <turnip_engine.hpp>
#include <platform/platform.hpp>

using namespace tur;

// View:
struct MainView : public View
{
public:
	void on_engine_startup() override
	{
		initialize_textures();

		// Camera:
		m_MainCamera.set_orthogonal(0.0f, 640.0f, 480.f, 0.f, -1.f, 1.f);

		// Scene:
		auto entity = m_Scene.add_entity();
		{
			glm::mat4 model(1.f);
			float scale = 100.f;
			model = glm::translate(model, glm::vec3(640.f / 2, 480.f / 2, 1.f));
			model = glm::scale(model, glm::vec3(scale, scale, 1.f));
			entity.add_component<TransformComponent>(model);
		}

		// Render System:
		m_RenderSystem.initialize(&m_Scene, &r_Engine->get_graphics_device(), &m_MainCamera);
		m_RenderSystem.get_renderer().set_clear_color({ 0.16f, 0.16f, 0.16f, 1.f });
	}

	void on_render() override
	{
		m_RenderSystem.render();
	};

private:
	void initialize_textures()
	{
		using namespace std::chrono_literals;

		auto& assetLibrary = r_Engine->get_asset_library();

		r_Engine->get_worker_pool().submit<asset_handle>([&]() {
			std::this_thread::sleep_for(700ms);
			return load_texture_asset(&assetLibrary, "res/textures/face.png");
		}, [&](asset_handle handle) {
			auto texture = assetLibrary.textures.get(handle);
			TUR_LOG_INFO("Loaded texture successfully: {}", texture.filepath.string());

			TextureDescriptor descriptor;
			{
				descriptor.format = TextureFormat::RGBA;
				descriptor.type = TextureType::TEXTURE_2D;
				descriptor.width = texture.width;
				descriptor.height = texture.height;
				descriptor.generateMipmaps = true;
			}

			m_Texture = r_Engine->get_graphics_device().create_texture(descriptor, texture);
		});
	}

private:
	Scene m_Scene;
	Camera m_MainCamera;
	texture_handle m_Texture;
	QuadRendererSystem m_RenderSystem;
};

int main()
{
	TurnipEngine engine;
	engine.initialize();

	engine.add_view(tur::make_unique<MainView>());

	engine.run();

	return 0;
}
