#pragma once
#include "Color.h"
#include "Maths/Transform.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Texture.h"
#include "Graphics/Mesh/Mesh.h"
#include "TofuRendererProps.h"

namespace tur
{
	class TofuRenderer
	{
		friend class TurnipEngine;

	private:
		static void Initialize(const TofuRendererProps& properties);

	public:
		static void Begin();

		static void DrawMesh(const Mesh& mesh, Texture& texture, Shader& shader);

		static void DrawQuad(const Transform& transform, Texture& texture, Shader& shader);

		static void End();

	public:
		static void SetColor(const Color& color);
	};
}