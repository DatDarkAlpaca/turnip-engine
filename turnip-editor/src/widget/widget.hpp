#pragma once
#include <turnip_engine.hpp>

class Widget
{
public:
	virtual ~Widget() = default;

public:
	inline void set_callback(tur::EventCallback&& callback) { this->callback = callback; }

	virtual void on_event(tur::Event& event) {}

protected:
	tur::EventCallback callback;
};