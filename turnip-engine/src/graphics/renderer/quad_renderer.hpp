#pragma once
#include "graphics/graphics.hpp"

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
		void initialize(GraphicsDevice* graphicsDevice);

		void render();

	public:
		void add_quad(const QuadData& quad)
		{
			m_Quads.push_back(quad);
		}

	private:
		void initialize_pipeline();

		void initialize_buffers();

	private:
		NON_OWNING GraphicsDevice* m_GraphicsDevice;
		tur_unique<CommandBuffer> m_Commands;

		std::vector<QuadData> m_Quads;

	private:
		buffer_handle indexBuffer;
		buffer_handle buffer;

		pipeline_handle pipeline;
	};
}