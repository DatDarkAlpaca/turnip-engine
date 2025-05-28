#pragma once
#include "graphics/graphics.hpp"
#include "utils/color.hpp"

namespace tur
{
	struct Renderer
	{
	public:
		virtual ~Renderer() = default;

	public:
		NON_OWNING GraphicsDevice* graphicsDevice = nullptr;
		NON_OWNING Camera* camera = nullptr;
		tur_unique<CommandBuffer> commands;

	public:
		Color clearColor;
		Viewport viewport;
		bool shouldClear = true;
	};

	inline void renderer_set_camera(Renderer* renderer, Camera* camera)
	{
		renderer->camera = camera;
	}
	inline void renderer_set_clear_color(Renderer* renderer, const Color& color)
	{
		renderer->clearColor = color;
	}
	inline void renderer_set_viewport(Renderer* renderer, const Viewport& viewport)
	{
		renderer->viewport = viewport;
	}
	inline void renderer_set_should_clear(Renderer* renderer, bool shouldClear)
	{
		renderer->shouldClear = shouldClear;
	}
}