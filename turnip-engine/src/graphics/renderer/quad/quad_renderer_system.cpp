#include "pch.hpp"
#include "quad_renderer_system.hpp"

namespace tur
{
	void initialize_quad_renderer_system(QuadRendererSystem& system, const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera, Scene* scene)
	{
		initialize_quad_renderer(system.renderer, configData, graphicsDevice, camera);
		system.scene = scene;
	}

	void quad_renderer_system_begin(QuadRendererSystem& system, render_target_handle textureHandle)
	{
		if (!system.scene)
			return TUR_LOG_ERROR("Quad renderer system has no set scene");

		if (!system.renderer.camera)
			return TUR_LOG_ERROR("Quad renderer system has no set camera");

		auto& registry = system.scene->get_registry();

		auto view0 = registry.view<TransformComponent, QuadTexture2D>();
		for (auto& [entity, transformComponent, textureComponent] : view0.each())
		{
			if (textureComponent.descriptorHandle == invalid_handle)
				textureComponent.descriptorHandle = system.renderer.graphicsDevice->create_descriptor_set(system.renderer.descriptor);

			QuadRenderer::Data quadData = {};
			quadData.transform = transformComponent.transform.transform();
			quadData.texture = textureComponent.textureHandle;
			quadData.descriptorSet = textureComponent.descriptorHandle;

			quad_renderer_add_quad(system.renderer, quadData);
		}

		quad_renderer_begin(system.renderer, textureHandle);
	}

	void quad_renderer_system_render(QuadRendererSystem& system)
	{
		quad_renderer_render(system.renderer);
	}

	void quad_renderer_system_end(QuadRendererSystem& system)
	{
		quad_renderer_end(system.renderer);

		quad_renderer_clear_quads(system.renderer);
	}
}

namespace tur
{
	void quad_renderer_system_set_camera(QuadRendererSystem& system, Camera* camera)
	{
		renderer_set_camera(&system.renderer, camera);
	}

	void quad_renderer_system_set_scene(QuadRendererSystem& system, Scene* scene)
	{
		system.scene = scene;
	}
}
