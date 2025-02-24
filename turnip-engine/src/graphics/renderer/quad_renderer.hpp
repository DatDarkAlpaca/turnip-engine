#pragma once
#include "quad_renderer_data.hpp"

#include "graphics/graphics.hpp"
#include "core/config/config_data.hpp"

namespace tur
{
	class QuadRenderer
	{
	public:
		struct QuadData
		{
			glm::mat4 transform;
			texture_handle texture;
		};

		struct Vertex
		{
			glm::vec3 position;
			glm::vec2 uvs;
		};

	public:
		void initialize(const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera);

		void render();

	public:
		void set_clear_color(const glm::vec4& color);

		void set_default_texture(texture_handle handle);

		void set_viewport(const Viewport& viewport);

	public:
		void add_quad(const QuadData& quad);

		void clear_quads();

	private:
		void initialize_pipeline();

		void initialize_buffers();

		void bind_mvp(const glm::mat4& transform);

	private:
		NON_OWNING GraphicsDevice* r_GraphicsDevice;
		NON_OWNING Camera* r_Camera;

		QuadRendererInformation m_QuadRendererInfo;

	private:
		tur_unique<CommandBuffer> m_Commands;
	
		std::vector<QuadData> m_Quads;
		glm::vec4 m_ClearColor;
		Viewport m_Viewport;

	private:
		buffer_handle uniformBuffer;
		buffer_handle indexBuffer;
		buffer_handle buffer;
		texture_handle defaultTexture = invalid_handle;

 		pipeline_handle pipeline;
	};
}