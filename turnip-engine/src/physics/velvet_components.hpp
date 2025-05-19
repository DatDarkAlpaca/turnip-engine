#pragma once
#include <glm/glm.hpp>
#include "core/scene/common_components.hpp"

namespace tur
{
	struct Rigidbody2DComponent
	{
	public:
		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const TransformComponent& transform, float mass = 1.0f, bool isStatic = false)
			: position(transform.transform.position)
			, mass(mass)
			, isStatic(isStatic)
		{

		}

	public:
		glm::vec2 position		= glm::vec2(0.0f);
		glm::vec2 oldPosition	= glm::vec2(0.0f);
		glm::vec2 force			= glm::vec2(0.0f);

		float mass = 1.0f;

		bool isStatic = false;
	};

	struct Collider2D
	{

	};
}