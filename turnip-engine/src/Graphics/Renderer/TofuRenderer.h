#pragma once
#include "Graphics/Mesh.h"
#include "OpenGL/Shader.h"

namespace tur
{
	// Tofu renderer is a immediate mode renderer. It will have the ability to draw instanced in the future.
	class TofuRenderer
	{
	public:
		TofuRenderer()
		{
			glEnable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthFunc(GL_LEQUAL);
		}

	public:
		void Begin()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void DrawMesh(const Mesh& mesh, Shader& shader)
		{
			shader.Bind();
			mesh.Bind();

			glDrawElements(GL_TRIANGLES, mesh.GetElementCount(), GL_UNSIGNED_INT, nullptr);
		}

		void End()
		{
			
		}

	public:
		void SetColor(const glm::vec4& color)
		{
			glClearColor(color.r, color.g, color.b, color.a);
		}
	};
}