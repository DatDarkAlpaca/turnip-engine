#pragma once
#include "ICamera.h"

#undef near
#undef far

namespace tur
{
	class PerspectiveCamera : public ICamera
	{
	public:
		PerspectiveCamera(float fov, float aspectRatio, float near, float far);

		PerspectiveCamera() = default;

	public:
		void Update() override;

	public:
		void SetProjection(float fov, float aspectRatio, float near, float far);

	public:
		inline glm::vec3& GetPosition() { return m_Position; }

		void SetPosition(const glm::vec3& position) { m_Position = position; }

	protected:
		glm::vec3 m_Right = glm::vec3(-1.f, 0, 0.f);
		glm::vec3 m_Up = glm::vec3(0, 1.f, 0.f);
		glm::vec3 m_Forward = glm::vec3(0, 0, -1.f);
		glm::vec3 m_Position = glm::vec3(0.f);
	};
}

#define near
#define far