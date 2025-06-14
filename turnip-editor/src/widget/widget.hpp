#pragma once
#include <turnip_engine.hpp>

class Widget
{
public:
	virtual ~Widget() = default;

public:
	inline void set_callback(tur::EventCallback&& callback) { this->callback = callback; }

	virtual void on_event(tur::Event& event) {}

public:
	void set_open(bool open) { isOpen = open; }
	bool isOpen = true;

protected:
	tur::EventCallback callback;	
};