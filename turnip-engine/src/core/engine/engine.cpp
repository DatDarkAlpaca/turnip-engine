#include "pch.hpp"
#include "engine.hpp"
#include "graphics/gui/gui.hpp"
#include "core/config/config_data.hpp"
#include "core/script/script_system.hpp"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace tur
{
	void TurnipEngine::initialize(const std::filesystem::path& configPath)
	{
		// Logger:
		initialize_logger_system();

		// Config:
		initialize_config_data(configPath);

		JsonReader configReader(configPath);
		auto readerResult = configReader.parse<ConfigData>();
		if (!readerResult.has_value())
			return;

		m_ConfigData = readerResult.value();

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

		m_QuadRendererSystem.initialize(m_ConfigData, &m_GraphicsDevice);
		m_InstancedQuadRendererSystem.initialize(m_ConfigData, &m_GraphicsDevice);

		// Scripting:
		ScriptSystem::initialize(m_ConfigData, this);	
	}
	void TurnipEngine::run()
	{
		on_engine_startup();

		while (is_open_window(&m_Window) && !m_RequestShutdown)
		{
			poll_events(&m_Window);
			m_WorkerPool.poll_tasks();

			on_update();

			on_render();

			m_GraphicsDevice.present();
		}

		on_engine_shutdown();

		shutdown_window(&m_Window);
		shutdown_windowing_system();
	}
	void TurnipEngine::shutdown()
	{
		m_RequestShutdown = true;
	}

	void TurnipEngine::add_view(tur_unique<View> view)
	{
		view->set_engine(this);
		view_system_add(&m_ViewSystem, std::move(view));
	}
	void TurnipEngine::remove_view(u32 handle)
	{
		view_system_remove(&m_ViewSystem, handle);
	}
	
	void TurnipEngine::on_engine_startup()
	{
		for (const auto& view : m_ViewSystem.views)
			view->on_engine_startup();
	}
	void TurnipEngine::on_render()
	{
		m_GraphicsDevice.begin_gui_frame();

		for (const auto& view : m_ViewSystem.views)
		{
			view->on_render_gui();
			view->on_render();
		}

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