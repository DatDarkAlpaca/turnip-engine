#include "pch.hpp"
#include "engine.hpp"
#include "graphics/gui/gui.hpp"
#include "core/config/config_data.hpp"

namespace tur
{
	void TurnipEngine::initialize(const std::filesystem::path& configPath)
	{
		// Logger:
		initialize_logger_system();

		// Config:
		initialize_config_data(configPath);

		ConfigReader configReader(configPath);
		m_ConfigData = configReader.parse<ConfigData>();

		// Worker Pool:
		m_WorkerPool.initialize();

		// Window:
		initialize_windowing_system();
		set_callback_window(&m_Window, BIND(&TurnipEngine::on_event, this));
		initialize_graphics_system(&m_Window, m_ConfigData.windowProperties, m_ConfigData.graphicsSpecification);

		// Gui:
		initialize_gui(&m_Window);
		
		// Graphics:
		m_GraphicsDevice.initialize(&m_Window, m_ConfigData);
		m_GraphicsDevice.initialize_gui_graphics_system();
	}

	void TurnipEngine::run()
	{
		on_engine_startup();

		while (is_open_window(&m_Window))
		{
			poll_events(&m_Window);
			m_WorkerPool.poll_tasks();

			on_update();

			on_render();

			on_render_gui();

			m_GraphicsDevice.present();
		}

		shutdown();
	}

	void TurnipEngine::shutdown()
	{
		on_engine_shutdown();

		shutdown_window(&m_Window);
		shutdown_windowing_system();
	}

	void TurnipEngine::add_view(tur_unique<View> view)
	{
		view->set_engine(this);
		view_system_add(&m_ViewSystem, std::move(view));
	}

	void TurnipEngine::on_engine_startup()
	{
		for (const auto& view : m_ViewSystem.views)
			view->on_engine_startup();
	}

	void TurnipEngine::on_render()
	{
		for (const auto& view : m_ViewSystem.views)
			view->on_render();
	}

	void TurnipEngine::on_render_gui()
	{
		m_GraphicsDevice.begin_gui_frame();
		
		for (const auto& view : m_ViewSystem.views)
			view->on_render_gui();
		
		 m_GraphicsDevice.end_gui_frame();
	}

	void TurnipEngine::on_update()
	{
		for (const auto& view : m_ViewSystem.views)
			view->on_update();
	}

	void TurnipEngine::on_event(Event& event)
	{
		for (const auto& view : m_ViewSystem.views)
			view->on_event(event);
	}

	void TurnipEngine::on_engine_shutdown()
	{
		for (const auto& view : m_ViewSystem.views)
			view->on_engine_shutdown();
	}
}