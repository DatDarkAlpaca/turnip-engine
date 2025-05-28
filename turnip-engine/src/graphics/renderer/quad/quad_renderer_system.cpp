#include "pch.hpp"
#include "quad_renderer_system.hpp"

namespace tur
{
	void initialize_quad_renderer_system(QuadRendererSystem& system, const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera, Scene* scene)
	{
		initialize_quad_renderer(system.renderer, configData, graphicsDevice, camera);
		system.scene = scene;
	}

	void quad_renderer_system_render(QuadRendererSystem& system)
	{
		if (!system.scene)
			return TUR_LOG_ERROR("Quad renderer system has no set scene");

		if(!system.renderer.camera)
			return TUR_LOG_ERROR("Quad renderer system has no set camera");

		auto& registry = system.scene->get_registry();

		auto view0 = registry.view<TransformComponent>(entt::exclude<TextureComponent>);
		for (auto [entity, transformComponent] : view0.each())
		{
			QuadRenderer::Data quadData = {};
			quadData.transform = transformComponent.transform.transform();
			quadData.texture = invalid_handle;

			quad_renderer_add_quad(system.renderer, quadData);
		}

		auto view1 = registry.view<TransformComponent, TextureComponent>();
		for (auto [entity, transformComponent, textureComponent] : view1.each())
		{
			const auto& texture = textureComponent.handle;

			QuadRenderer::Data quadData = {};
			quadData.transform = transformComponent.transform.transform();
			quadData.texture = texture;

			quad_renderer_add_quad(system.renderer, quadData);
		}

		quad_renderer_render(system.renderer);
		quad_renderer_clear_quads(system.renderer);
	}
}

namespace tur
{
	void quad_renderer_system_set_camera(QuadRendererSystem& system, Camera* camera)
	{
		quad_renderer_set_camera(system.renderer, camera);
	}

	void quad_renderer_system_set_scene(QuadRendererSystem& system, Scene* scene)
	{
		system.scene = scene;
	}
}
