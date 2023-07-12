#pragma once
#include "pch.h"
#include "OrthographicCamera.h"

#undef near
#undef far

namespace tur
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
	{
		SetProjection(left, right, bottom, top, near, far);
	}

	void OrthographicCamera::UpdateCamera(int left, int right, int bottom, int top)
	{
		glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float near, float far)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, near, far);
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
	}
}

#define near
#define far