#pragma once
#include "pch.h"

namespace tur
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normals = glm::vec3(0.f);
		glm::vec4 color = glm::vec4(0.f);
		glm::vec2 uvs = glm::vec2(0.f);
	};
}