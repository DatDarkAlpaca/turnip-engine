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

		m_Renderer.initialize(&r_Engine->get_graphics_device());
		m_Renderer.add_quad(QuadRenderer::QuadData{ glm::mat4(), m_Texture });
	}

	void on_render() override
	{
		m_Renderer.render();
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
	texture_handle m_Texture;
	QuadRenderer m_Renderer;
};

int main()
{
	TurnipEngine engine;
	engine.initialize();

	engine.add_view(tur::make_unique<MainView>());

	engine.run();

	return 0;
}
