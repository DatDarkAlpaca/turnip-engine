#pragma once
#include "core/scene/common_components.hpp"
#include "core/scene/scene.hpp"
#include "instanced_quad_renderer.hpp"

namespace tur
{
	struct InstancedQuadSystem
	{
		InstancedQuadRenderer renderer;
		NON_OWNING Scene* scene = nullptr;
	};

	void initialize_instanced_quad_system(InstancedQuadSystem& system, const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera = nullptr, Scene* scene = nullptr);
	
	void instanced_quad_system_begin(InstancedQuadSystem& system);
	void instanced_quad_system_render(InstancedQuadSystem& system, render_target_handle handle = invalid_handle);
	void instanced_quad_system_end(InstancedQuadSystem& system);

	void instanced_quad_system_set_camera(InstancedQuadSystem& system, Camera* camera);
	void instanced_quad_system_set_scene(InstancedQuadSystem& system, Scene* scene);
}