#pragma once
#include <imgui.h>
#include <turnip_engine.hpp>

struct ProjectView : public tur::View
{
public:
	void on_engine_startup();

public:
	void on_update();
	void on_render_gui();

private:
	bool m_IsOpen = true;
};
