#pragma once
#include "WindowProperties.h"

namespace tur
{
	class IWindow
	{
	public:
		IWindow(const WindowProperties& properties)
			: properties(properties) { }

		virtual ~IWindow() = default;

	public:
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