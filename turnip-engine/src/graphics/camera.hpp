#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace tur
{
	class Camera
	{
	public:
		void set_orthogonal(float left, float right, float bottom, float top, float zNear, float zFar)
		{
			m_Projection = glm::ortho(left, right, bottom, top, zNear, zFar);
		}

		void set_perspective(float fov, float aspectRatio, float zNear, float zFar)
		{
			m_Projection = glm::perspective(fov, aspectRatio, zNear, zFar);
		}

		void set_position(const glm::vec3& position)
		{
			m_Position = position;
		}

		void set_target(const glm::vec3& target)
		{
			m_Target = target;
		}

		void update_view()
		{
			m_View = glm::lookAt(m_Position, m_Target, Camera::s_Up);
		}

	public:
		glm::mat4 view() { return m_View; }
		glm::mat4 projection() { return m_Projection; }

		glm::vec3 position() const { return m_Position; }
		glm::vec3 target() const { return m_Target; }

	private:
		static inline const glm::vec3 s_Up = glm::vec3(0.0f, 1.0f, 0.f);
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec3 m_Target = glm::vec3(0.0f);

	private:
		glm::mat4 m_Projection = glm::mat4(1.f);
		glm::mat4 m_View = glm::mat4(1.f);
	};
}