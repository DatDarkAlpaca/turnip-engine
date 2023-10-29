#pragma once
#include "Core/NonCopyable.h"
#include "Core/Window/WindowProperties.h"

namespace tur
{
	class Window : public NonCopyable
	{
	public:
		virtual ~Window() = default;

	public:
		static tur::tur_unique<Window> Create(const WindowProperties& properties = {});

	public:
		virtual void PollEvents() = 0;

		virtual void SetEventCallback(const FnEventCallback& callback) = 0;

	public:
		virtual void Hide() = 0;

		virtual void Show() = 0;

	public:
		virtual bool IsOpen() const = 0;

		virtual glm::vec2 GetPosition() const = 0;

		virtual void SetPosition(const glm::vec2& position) = 0;

		virtual void SetSizeLimits(const glm::vec2& minimumSize, const glm::vec2& maximumSize) = 0;
	};
}