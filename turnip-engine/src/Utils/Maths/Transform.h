#pragma once

namespace tur
{
	class Transform
	{
	public:
		Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
			: position(position), rotation(rotation), scale(scale) { }

		Transform(const Transform&) = default;

		Transform() = default;

	public:
		glm::vec3 position = glm::vec3(0.f), rotation = glm::vec3(0.f), scale = glm::vec3(1.f);
	};
}