#pragma once
#include "pch.h"
#include "ICamera.h"

#undef near
#undef far

namespace tur
{
	class OrthographicCamera : public ICamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float near = -1.f, float far = 1.f);

	public:
		void UpdateCamera(int left, int right, int bottom, int top);

	public:
		void SetPosition(const glm::vec3& position);
		
		void SetProjection(float left, float right, float bottom, float top, float near = -1.f, float far = 1.f);

	public:
		inline glm::vec3 GetPosition() const { return m_Position; }

	private:
		const glm::vec3 m_Front = glm::vec3(0.f, 0.f, -1.f);
		const glm::vec3 m_Up = glm::vec3(0.f, 1.f, 0.f);
		glm::vec3 m_Position = glm::vec3(0.f);
	};
}

#define near
#define far