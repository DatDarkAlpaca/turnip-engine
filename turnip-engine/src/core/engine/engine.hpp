#pragma once
#include <filesystem>

#include "core/assets/asset_library.hpp"
#include "core/worker/worker_pool.hpp"
#include "core/event/events.hpp"
#include "core/view/view.hpp"

#include "graphics/renderer/quad/quad_renderer_system.hpp"
#include "graphics/renderer/instanced/instanced_quad_renderer_system.hpp"

#include "graphics/graphics.hpp"
#include "platform/platform.hpp"

namespace tur
{
	class TurnipEngine
	{
	public:
		void initialize(const std::filesystem::path& configPath);
		void run();
		void shutdown();

	public:	 
		void add_view(tur_unique<View> view);
		void remove_view(u32 handle);

	private:
		void on_engine_startup();
		void on_render();
		void on_update();
		void on_event(Event& event);
		void on_engine_shutdown();

	public:
		Window& get_window() { return m_Window; }
		ViewSystem& get_view_system() { return m_ViewSystem; }
		AssetLibrary& get_asset_library() { return m_AssetLibrary; }
		WorkerPool& get_worker_pool() { return m_WorkerPool; }
		GraphicsDevice& get_graphics_device() { return m_GraphicsDevice; }

		QuadRendererSystem& get_quad_renderer_system() { return m_QuadRendererSystem; }
		InstancedQuadRendererSystem& get_instanced_quad_renderer_system() { return m_InstancedQuadRendererSystem; }
				
		const ConfigData& get_config_data() const { return m_ConfigData; }

	private:
		Window m_Window;
		ViewSystem m_ViewSystem;
		AssetLibrary m_AssetLibrary;
		ConfigData m_ConfigData;
		WorkerPool m_WorkerPool;
		GraphicsDevice m_GraphicsDevice;

		QuadRendererSystem m_QuadRendererSystem;
		InstancedQuadRendererSystem m_InstancedQuadRendererSystem;

	private:
		bool m_RequestShutdown = false;
	};
}