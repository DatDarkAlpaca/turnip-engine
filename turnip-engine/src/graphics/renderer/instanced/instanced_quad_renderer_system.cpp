#include "pch.hpp"
#include "instanced_quad_renderer_system.hpp"

namespace tur
{
	void initialize_instanced_quad_system(InstancedQuadSystem& system, const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera, Scene* scene)
	{
		initialize_instanced_quad_renderer(system.renderer, configData, graphicsDevice, camera);
		system.scene = scene;
	}

	void instanced_quad_system_begin(InstancedQuadSystem& system)
	{
		if (!system.scene)
			return TUR_LOG_ERROR("Instanced quad renderer system has no set scene");

		if (!system.renderer.camera)
			return TUR_LOG_ERROR("Instanced quad renderer system has no set camera");

		auto& registry = system.scene->get_registry();

		auto view = registry.view<TransformComponent, InstancedQuadComponent>();
		for (auto [entity, transformComponent, instancedQuadComponent] : view.each())
		{
			if (!instancedQuadComponent.dirtyFlag)
				continue;

			InstanceData data;
			{
				data.position = transformComponent.transform.position;
				data.scale = glm::vec2(transformComponent.transform.scale);
				data.textureLayer = instancedQuadComponent.textureLayer;
			}

			if (instancedQuadComponent.instanceIndex == invalid_handle)
				instancedQuadComponent.instanceIndex = instanced_quad_renderer_add_quad(system.renderer, data);
			else
				instanced_quad_renderer_update_quad(system.renderer, data, instancedQuadComponent.instanceIndex);
		}

		instanced_quad_renderer_begin(system.renderer);
	}

	void instanced_quad_system_render(InstancedQuadSystem& system, render_target_handle handle)
	{
		instanced_quad_renderer_render(system.renderer, handle);
	}

	void instanced_quad_system_end(InstancedQuadSystem& system)
	{
		instanced_quad_renderer_end(system.renderer);
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