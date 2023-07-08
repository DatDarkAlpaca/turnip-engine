#pragma once
#include "pch.h"
#include "Maths/Transform.h"

namespace tur
{
	struct ComponentTransform
	{
	public:
		ComponentTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
			: transform(position, rotation, scale) { }
		
		ComponentTransform(const Transform& transform)
			: transform(transform) { }

		ComponentTransform() = default;

	public:
		Transform transform;
	};

	struct ComponentTexture
	{
	public:
		ComponentTexture(const std::shared_ptr<Texture>& texture)
			: texture(texture) { }

		std::weak_ptr<Texture> texture;
	};
}