#pragma once
#include "quad_renderer_data.hpp"
#include "graphics/renderer/renderer.hpp"

#include "core/config/config_data.hpp"
#include "core/event/events.hpp"

namespace tur
{
	struct QuadRenderer : public Renderer
	{
	public:
		struct Data
		{
			glm::mat4 transform;
			texture_handle texture = invalid_handle;
			descriptor_set_handle descriptorSet = invalid_handle;
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
		std::vector<Data> quads;

	public:
		descriptor_handle descriptor		= invalid_handle;
		buffer_handle uniformBuffer			= invalid_handle;
		buffer_handle indexBuffer			= invalid_handle;
		buffer_handle buffer				= invalid_handle;
		pipeline_handle pipeline			= invalid_handle;

	public:
		texture_handle defaultTexture		= invalid_handle;
		descriptor_set_handle defaultDescriptorSet = invalid_handle;
	};

	void initialize_quad_renderer(QuadRenderer& renderer, const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera);
	
	void quad_renderer_begin(QuadRenderer& renderer, render_target_handle renderTarget = invalid_handle);
	void quad_renderer_render(QuadRenderer& renderer);
	void quad_renderer_end(QuadRenderer& renderer);

	void quad_renderer_set_default_texture(QuadRenderer& renderer, texture_handle textureHandle);

	void quad_renderer_add_quad(QuadRenderer& renderer, QuadRenderer::Data& quad);
	void quad_renderer_clear_quads(QuadRenderer& renderer);
}