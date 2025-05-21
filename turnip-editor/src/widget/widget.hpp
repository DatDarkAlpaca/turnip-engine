#pragma once
#include <turnip_engine.hpp>

namespace tur
{
	class Widget
	{
	public:
		virtual ~Widget() = default;

	public:
		inline void set_callback(EventCallback&& callback) { this->callback = callback; }

		virtual void on_event(Event& event) { }

	protected:
		EventCallback callback;
	};
}