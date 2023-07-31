#pragma once
#include "WindowProperties.h"
#include "Core/Event/IEvent.h"

namespace tur
{
	class IWindow
	{
	protected:
		using FnEventCallback = std::function<void(IEvent&)>;

	public:
		IWindow(const WindowProperties& properties)
			: properties(properties) { }

		virtual ~IWindow() = default;

	public:
		virtual void SetEventCallback(const FnEventCallback& eventCallback) = 0;

		virtual void PollEvents() = 0;

	public:
		virtual void Show() = 0;

		virtual void Hide() = 0;

	public:
		virtual bool IsOpen() const = 0;

	public:
		virtual void* GetWindow() const = 0;

	protected:
		WindowProperties properties;
	};
}