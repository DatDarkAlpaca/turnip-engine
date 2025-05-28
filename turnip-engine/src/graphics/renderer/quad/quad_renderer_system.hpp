#pragma once
#include "core/scene/common_components.hpp"
#include "core/scene/scene.hpp"
#include "graphics/camera.hpp"
#include "quad_renderer.hpp"

namespace tur
{
	struct QuadRendererSystem
	{
	public:
		QuadRenderer renderer;
		NON_OWNING Scene* scene = nullptr;
	};

	void initialize_quad_renderer_system(QuadRendererSystem& system, const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera = nullptr, Scene* scene = nullptr);
	
	void quad_renderer_system_begin(QuadRendererSystem& system);
	void quad_renderer_system_render(QuadRendererSystem& system, render_target_handle handle = invalid_handle);
	void quad_renderer_system_end(QuadRendererSystem& system);

	void quad_renderer_system_set_camera(QuadRendererSystem& system, Camera* camera);
	void quad_renderer_system_set_scene(QuadRendererSystem& system, Scene* scene);
}