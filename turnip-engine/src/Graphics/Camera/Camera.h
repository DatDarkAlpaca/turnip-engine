#pragma once
#include "pch.h"

#undef near
#undef far

namespace tur
{
	class Orthocamera
	{
	public:
		Orthocamera(int left, int right, int bottom, int top, int near, int far)
		{
			SetProjection(left, right, bottom, top, near, far);
		}

	public:
		void SetProjection(int left, int right, int bottom, int top, int near, int far)
		{
			m_ProjectionMatrix = glm::ortho(left, right, bottom, top, near, far);
		}

		void UpdateCamera(int left, int right, int bottom, int top)
		{
			glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		}

	public:
		void SetPosition(const glm::vec3& position)
		{
			m_Position = position;
		}

	public:
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

	private:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.f), m_ViewMatrix = glm::mat4(1.f);
		const glm::vec3 m_Front = glm::vec3(0.f, 0.f, -1.f);
		const glm::vec3 m_Up = glm::vec3(0.f, 1.f, 0.f);
		glm::vec3 m_Position = glm::vec3(0.f);
	};
}

#define near
#define far