#pragma once
#include "pch.h"

namespace tur
{
	class ICamera
	{
	public:
		virtual ~ICamera() = default;

	public:
		inline const glm::mat4& GetProjectionMatrix() const { return projectionMatrix; }

		inline const glm::mat4& GetViewMatrix() const { return viewMatrix; }

	protected:
		glm::mat4 projectionMatrix = glm::mat4(1.f), viewMatrix = glm::mat4(1.f);
	};
}