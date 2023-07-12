#include "pch.h"
#include "TofuRenderer.h"

#include "Graphics/Vertex.h"

namespace tur
{
	void TofuRenderer::Initialize(const TofuRendererProps& properties)
	{
		if (properties.enableBlend)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		if (properties.enableDepthTest)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
		}
	}

	void TofuRenderer::Begin(ICamera* camera)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_RendererData.projectionMatrix = camera->GetProjectionMatrix();
		m_RendererData.viewMatrix = camera->GetViewMatrix();
	}

	void TofuRenderer::DrawMesh(const Mesh& mesh, Texture& texture, Shader& shader)
	{
		texture.Bind();
		mesh.Bind();
		shader.Bind();

		shader.SetMatrix4f("u_view", m_RendererData.viewMatrix);
		shader.SetMatrix4f("u_projection", m_RendererData.projectionMatrix);

		glDrawElements(GL_TRIANGLES, mesh.GetElementCount(), GL_UNSIGNED_INT, nullptr);
	}

	void TofuRenderer::DrawQuad(const Transform& transform, Texture& texture, Shader& shader)
	{
		Mesh mesh;
		std::vector<Vertex> vertices
		{
			{ {  1.f,  1.f, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 1.0f, 1.0f, 0.5f, 1.0f }, { 1.0f, 1.0f } },
			{ { -1.f,  1.f, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 0.4f, 1.0f, 0.2f, 1.0f }, { 0.0f, 1.0f } },
			{ { -1.f, -1.f, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 0.5f, 1.0f, 0.5f, 1.0f }, { 0.0f, 0.0f } },
			{ {  1.f, -1.f, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 1.0f, 0.6f, 0.3f, 1.0f }, { 1.0f, 0.0f } }
		};
		std::vector<U32> indices { 0, 1, 2, 2, 3, 0 };

		mesh.Initialize(vertices, indices);

		texture.Bind();
		shader.Bind();
		mesh.Bind();

		glm::mat4 model(1.f);
		glm::mat4 rotation = glm::toMat4(glm::quat(transform.rotation));

		model = glm::translate(model, transform.position);
		model = glm::scale(model, transform.scale);
		model *= rotation;

		shader.SetMatrix4f("u_model", model);
		shader.SetMatrix4f("u_view", m_RendererData.viewMatrix);
		shader.SetMatrix4f("u_projection", m_RendererData.projectionMatrix);

		glDrawElements(GL_TRIANGLES, mesh.GetElementCount(), GL_UNSIGNED_INT, nullptr);

		mesh.Unbind();
		mesh.Destroy();
	}

	void TofuRenderer::End()
	{
	}

	void TofuRenderer::SetColor(const Color& color)
	{
		glClearColor(color.r(), color.g(), color.b(), color.a());
	}
}