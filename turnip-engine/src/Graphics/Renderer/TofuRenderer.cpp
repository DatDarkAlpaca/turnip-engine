#include "pch.h"
#include "TofuRenderer.h"

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

	void TofuRenderer::Begin()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void TofuRenderer::DrawMesh(const Mesh& mesh, Texture& texture, Shader& shader)
	{
		texture.Bind();
		mesh.Bind();
		shader.Bind();

		glDrawElements(GL_TRIANGLES, mesh.GetElementCount(), GL_UNSIGNED_INT, nullptr);
	}

	void TofuRenderer::End()
	{
	}

	void TofuRenderer::SetColor(const Color& color)
	{
		glClearColor(color.r(), color.g(), color.b(), color.a());
	}
}