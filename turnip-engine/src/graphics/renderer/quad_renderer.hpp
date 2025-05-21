#pragma once
#include "quad_renderer_data.hpp"

#include "graphics/graphics.hpp"
#include "core/config/config_data.hpp"
#include "core/event/events.hpp"

#include "utils/color.hpp"

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

		struct UBO
		{
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 projection;
		};

	public:
		void initialize(const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera);

		void on_event(Event& event);

		void render();

	public:
		void set_camera(Camera* camera);

		void set_clear_color(const Color& color);
		void set_default_texture(texture_handle handle);
		void set_viewport(const Viewport& viewport);
		void set_render_target_texture(texture_handle handle = invalid_handle);

	public:
		void add_quad(const QuadData& quad);
		void clear_quads();

	private:
		void initialize_pipeline();

		void initialize_buffers();

		void bind_mvp(const glm::mat4& transform);

	private:
		NON_OWNING GraphicsDevice* r_GraphicsDevice = nullptr;
		NON_OWNING Camera* r_Camera = nullptr;
		tur_unique<CommandBuffer> m_Commands;

		QuadRendererInformation m_QuadRendererInfo;

	private:
		std::vector<QuadData> m_Quads;
		Color m_ClearColor;
		Viewport m_Viewport;

	private:
		buffer_handle uniformBuffer = invalid_handle;
		buffer_handle indexBuffer = invalid_handle;
		buffer_handle buffer = invalid_handle;
		texture_handle defaultTexture = invalid_handle;
 		pipeline_handle pipeline = invalid_handle;

		render_target_handle renderTarget = invalid_handle;
	};
}