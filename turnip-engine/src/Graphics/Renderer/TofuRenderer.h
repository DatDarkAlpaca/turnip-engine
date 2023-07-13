#pragma once
#include "Color.h"
#include "Maths/Transform.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Texture.h"
#include "Graphics/Mesh/Mesh.h"
#include "TofuRendererProps.h"
#include "Camera/ICamera.h"

namespace tur
{
	class TofuRenderer
	{
		friend class TurnipEngine;

	public:
		static void Initialize(const TofuRendererProps& properties);

	public:
		static void Begin(ICamera* camera);

		static void DrawMesh(const Mesh& mesh, Texture& texture, Shader& shader);

		static void DrawQuad(const Transform& transform, Texture& texture, Shader& shader);

		static void End();

	public:
		static void SetColor(const Color& color);

	private:
		struct RendererData
		{
			glm::mat4 viewMatrix, projectionMatrix;
		} static inline m_RendererData;
	};
}