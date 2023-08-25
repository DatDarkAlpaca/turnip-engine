#pragma once
#include "WindowProperties.h"
#include "Core/Event/Event.h"

namespace tur
{
	template<typename Derived>
	class BaseWindow
	{
	protected:
		using FnEventCallback = std::function<void(IEvent&)>;

	public:
		BaseWindow(const WindowProperties& properties)
			: properties(properties) { }

	public:
		void SetEventCallback(const FnEventCallback& eventCallback)
		{
			static_cast<Derived*>(this)->SetEventCallback(eventCallback);
		}

		void pollEvents()
		{
			static_cast<Derived*>(this)->pollEvents();
		}

	public:
		void Show()
		{
			static_cast<Derived*>(this)->Show();
		}

		void Hide()
		{
			static_cast<Derived*>(this)->Hide();
		}

	public:
		bool IsOpen() const
		{
			return static_cast<Derived*>(this)->IsOpen();
		}

	public:
		void* Get() const
		{
			return static_cast<Derived*>(this)->Get();
		}

	protected:
		WindowProperties properties;
	};
}