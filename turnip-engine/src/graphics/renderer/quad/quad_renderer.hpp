#pragma once
#include "quad_renderer_data.hpp"

#include "graphics/graphics.hpp"
#include "core/config/config_data.hpp"
#include "core/event/events.hpp"

#include "utils/color.hpp"

namespace tur
{
	struct QuadRenderer
	{
	public:
		struct Data
		{
			glm::mat4 transform;
			texture_handle texture;
		};

		struct Vertex
		{
			glm::vec3 position;
			glm::vec2 uvs;
		};

		struct UBO
		{
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 projection;
		};

	public:
		NON_OWNING GraphicsDevice* graphicsDevice = nullptr;
		NON_OWNING Camera* camera = nullptr;
		tur_unique<CommandBuffer> commands;

	public:
		std::vector<Data> quads;
		Color clearColor;
		Viewport viewport;

	public:
		buffer_handle uniformBuffer			= invalid_handle;
		buffer_handle indexBuffer			= invalid_handle;
		buffer_handle buffer				= invalid_handle;
		texture_handle defaultTexture		= invalid_handle;
		pipeline_handle pipeline			= invalid_handle;
		render_target_handle renderTarget	= invalid_handle;
	};

	void initialize_quad_renderer(QuadRenderer& renderer, const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera);
	void quad_renderer_on_event(QuadRenderer& renderer, Event& event);
	void quad_renderer_render(QuadRenderer& renderer);

	void quad_renderer_set_camera(QuadRenderer& renderer, Camera* camera);
	void quad_renderer_set_clear_color(QuadRenderer& renderer, const Color& color);
	void quad_renderer_set_default_texture(QuadRenderer& renderer, texture_handle handle);
	void quad_renderer_set_viewport(QuadRenderer& renderer, const Viewport& viewport);
	void quad_renderer_set_render_target_texture(QuadRenderer& renderer, texture_handle handle = invalid_handle);

	void quad_renderer_add_quad(QuadRenderer& renderer, const QuadRenderer::Data& quad);
	void quad_renderer_clear_quads(QuadRenderer& renderer);
}