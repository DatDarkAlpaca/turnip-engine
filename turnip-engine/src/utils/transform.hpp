#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace tur
{
	struct Transform
	{
	public:
		Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
			: position(position)
			, rotation(rotation)
			, scale(scale)
		{

		}

		Transform(const glm::mat4& transform)
			: m_Transform(transform)
		{

		}
		
		Transform() = default;

	public:
		glm::mat4& transform()
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, position);
			
			glm::quat quaternion = glm::quat(glm::radians(rotation));
			model *= glm::mat4_cast(quaternion);

			model = glm::scale(model, scale);
			
			m_Transform = model;
			return m_Transform;
		}

	public:
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale	   = glm::vec3(1.0f);

	private:
		glm::mat4 m_Transform = glm::mat4(1.0f);
	};
}