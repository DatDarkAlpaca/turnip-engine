#include "pch.hpp"
#include "engine.hpp"
#include "graphics/gui/gui.hpp"
#include "core/config/config_data.hpp"
#include "core/script/script_system.hpp"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace tur::engine
{
	static void on_startup(TurnipEngine& data)
	{
		for (const auto& view : data.viewSystem.views)
			view->on_engine_startup();
	}

	static void on_render(TurnipEngine& data)
	{
		data.graphicsDevice.begin_recording();
		
		// GUI:
		{
			data.guiSystem->begin_frame();

			for (const auto& view : data.viewSystem.views)
			{
				view->on_render_gui();
			}

			data.guiSystem->render();
			data.guiSystem->end_frame();
		}

		// Rendering:
		for (const auto& view : data.viewSystem.views)
			view->on_render();

		data.graphicsDevice.submit();
		data.graphicsDevice.present();
	}
	
	static void on_update(TurnipEngine& data)
	{
		for (const auto& view : data.viewSystem.views)
			view->on_update();
	}

	static void on_event(TurnipEngine& data, Event& event)
	{
		for (const auto& view : data.viewSystem.views)
			view->on_event(event);
	}

	static void on_shutdown(TurnipEngine& data)
	{
		for (const auto& view : data.viewSystem.views)
			view->on_engine_shutdown();
	}
}

namespace tur
{
	void initialize_turnip_engine(TurnipEngine& data, const std::filesystem::path& configPath)
	{
		// Config:
		initialize_config_data(configPath);

		// Logger:
		initialize_logger_system();

		auto readerResult = json_parse_file<ConfigData>(configPath);
		if (!readerResult.has_value())
		{
			TUR_LOG_CRITICAL("Failed to parse the configuration from the specified filepath: {}", configPath.string());
			return;
		}

		data.configData = readerResult.value();

		// Scripting:
		ScriptSystem::initialize(data.configData, &data);

		// Worker Pool:
		data.workerPool.initialize();

		// Window:
		initialize_windowing_system();
		set_callback_window(&data.window, [&](Event& event) { engine::on_event(data, event); });
		initialize_graphics_system(&data.window, data.configData);
		
		// Graphics:
		data.graphicsDevice.initialize(&data.window, data.configData);
		
		// GUI:
		initialize_gui_system();
		data.guiSystem = tur::make_unique<GUISystem>(data.graphicsDevice.create_gui_system());
		data.guiSystem->initialize();

		// Renderers:
		initialize_quad_renderer_system(data.quadRendererSystem, data.configData, &data.graphicsDevice);
		//initialize_instanced_quad_system(data.instancedQuadSystem, data.configData, &data.graphicsDevice);
	}
	
	void turnip_engine_run(TurnipEngine& data)
	{
		engine::on_startup(data);

		while (is_open_window(&data.window) && !data.shutdownRequested)
		{
			poll_events(&data.window);
			data.workerPool.poll_tasks();

			engine::on_update(data);

			engine::on_render(data);
		}

		engine::on_shutdown(data);

		shutdown_window(&data.window);
		shutdown_windowing_system();
	}

	void turnip_engine_shutdown(TurnipEngine& data)
	{
		data.shutdownRequested = true;
	}
}

namespace tur
{
	void engine_add_view(TurnipEngine& data, tur_unique<View> view)
	{
		view->set_engine(&data);
		view_system_add(&data.viewSystem, std::move(view));
	}

	void engine_remove_view(TurnipEngine& data, view_handle handle)
	{
		view_system_remove(&data.viewSystem, handle);
	}
}