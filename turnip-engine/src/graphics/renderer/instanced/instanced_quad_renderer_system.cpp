#include "pch.hpp"
#include "instanced_quad_renderer_system.hpp"

namespace tur
{
	void initialize_instanced_quad_system(InstancedQuadSystem& system, const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera, Scene* scene)
	{
		initialize_instanced_quad_renderer(system.renderer, configData, graphicsDevice, camera);
		system.scene = scene;
	}

	void instanced_quad_system_render(InstancedQuadSystem& system)
	{
		TUR_LOG_CRITICAL("Instanced Quad Renderering is not available as an engine system yet");

		if (!system.scene)
			return TUR_LOG_ERROR("Instanced quad renderer system has no set scene");

		if (!system.renderer.camera)
			return TUR_LOG_ERROR("Instanced quad renderer system has no set camera");

		auto& registry = system.scene->get_registry();

		auto view0 = registry.view<TransformComponent>(entt::exclude<TextureComponent>);
		for (auto [entity, transformComponent] : view0.each())
		{
			// TODO: implement
		}

		auto view1 = registry.view<TransformComponent, TextureComponent>();
		for (auto [entity, transformComponent, textureComponent] : view1.each())
		{
			// TODO: implement
		}

		instanced_quad_renderer_render(system.renderer);
	}
}

namespace tur
{
	void instanced_quad_system_set_camera(InstancedQuadSystem& system, Camera* camera)
	{
		renderer_set_camera(&system.renderer, camera);
	}

	void instanced_quad_system_set_scene(InstancedQuadSystem& system, Scene* scene)
	{
		system.scene = scene;
	}
}