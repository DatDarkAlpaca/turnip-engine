#include "pch.h"
#include "PerspectiveCamera.h"

#undef near
#undef far

namespace tur
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float near, float far)
	{
		SetProjection(fov, aspectRatio, near, far);
	}

	void PerspectiveCamera::Update()
	{
		viewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float near, float far)
	{
		projectionMatrix = glm::perspective(fov, aspectRatio, near, far);
	}
}

#define near
#define far