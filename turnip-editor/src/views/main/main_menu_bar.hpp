#pragma once
#include <turnip_engine.hpp>

class MainView;

struct MainMenuBar
{
public:
	void initialize(NON_OWNING MainView* mainView);

	void on_render();

private:
	void on_file_new();
	void on_file_open();
	void on_file_save();
	void on_file_save_as();
	void on_file_close();

private:
	NON_OWNING MainView* r_MainView = nullptr;
};