#pragma once
#include "Core/NonCopyable.h"
#include "Core/Window/WindowProperties.h"

namespace tur
{
	template<typename T>
	class WindowBase : public NonCopyable
	{
		using WindowTraits = T;
		WindowTraits window;

	public:
		WindowBase() = default;

	public:
		void Initialize(const WindowProperties& properties)
		{
			window.Initialize(properties);
		}

		void SetEventCallback(const FnEventCallback& callback)
		{
			window.SetEventCallback(callback);
		}

		void SetProperties(const WindowProperties& properties)
		{
			window.SetProperties(properties);
		}

		void PollEvents()
		{
			window.PollEvents();
		}

	public:
		void Hide()
		{
			window.Hide();
		}

		void Show()
		{
			window.Show();
		}

	public:
		WindowProperties GetProperties() const 
		{ 
			return window.GetProperties(); 
		}

		bool IsOpen() const
		{
			return window.IsOpen();
		}

		void* GetHandle() const
		{
			return window.GetHandle();
		}
	};
}