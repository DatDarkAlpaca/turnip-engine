#include "pch.h"
#include "Mesh.h"

namespace tur
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<U32>& indices)
		: m_VBO(GL_ARRAY_BUFFER),
		  m_EBO(GL_ELEMENT_ARRAY_BUFFER)
	{
		Initialize(vertices, indices);
	}
	
	Mesh::Mesh()
		: m_VBO(GL_ARRAY_BUFFER),
		  m_EBO(GL_ELEMENT_ARRAY_BUFFER)
	{
	}

	Mesh::~Mesh()
	{
		if (!m_VAO.IsValid() || !m_VBO.IsValid() || !m_EBO.IsValid())
			return;

		TUR_CORE_WARN("Mesh instance destructor called, but object was never destroyed. It is advised to use Destroy() beforehand.");
	}

	void Mesh::Initialize(const std::vector<Vertex>& vertices, const std::vector<U32>& indices)
	{
		TUR_WRAP_DEBUG(
			if (!m_Vertices.empty() || !m_Indices.empty())
				TUR_CORE_WARN("Potentially initializing an already configured mesh.");
		);

		m_Vertices = vertices;
		m_Indices = indices;

		Setup();
	}

	void Mesh::Destroy()
	{
		m_VBO.Destroy();
		m_EBO.Destroy();
		m_VAO.Destroy();
	}

	void Mesh::Setup()
	{
		m_VAO.Bind();

		m_EBO.Bind();
		m_EBO.Initialize(m_Indices.size() * sizeof(U32), &m_Indices[0], GL_STATIC_DRAW);

		m_VBO.Bind();
		m_VBO.Initialize(m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

		m_VAO.AddAttribute({ 0, 3, GL_FLOAT }); // Position
		m_VAO.AddAttribute({ 1, 3, GL_FLOAT }); // Normals
		m_VAO.AddAttribute({ 2, 4, GL_FLOAT }); // Color
		m_VAO.AddAttribute({ 3, 2, GL_FLOAT }); // UVs

		m_VAO.FlushLayout();

		m_VBO.Unbind();
		m_VAO.Unbind();
	}
}