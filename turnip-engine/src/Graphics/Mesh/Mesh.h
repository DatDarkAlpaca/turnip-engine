#pragma once
#include "Vertex.h"
#include "OpenGL/Buffer.h"
#include "OpenGL/VertexArray.h"

namespace tur
{
	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<U32>& indices);

		Mesh();

		~Mesh();

	private:
		void Setup();

	public:
		void Initialize(const std::vector<Vertex>& vertices, const std::vector<U32>& indices);

		void Destroy();

	public:
		inline U32 GetElementCount() const { return (U32)m_Indices.size(); }

	public:
		inline void Bind() const { m_VAO.Bind(); m_VBO.Bind(); }

		inline void Unbind() const { m_VBO.Unbind(); m_EBO.Unbind(); m_VAO.Unbind(); }

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<U32> m_Indices;

		VertexArray m_VAO;
		Buffer m_VBO, m_EBO;
	};
}