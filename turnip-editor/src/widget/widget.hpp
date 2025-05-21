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

	protected:
		EventCallback callback;
	};
}