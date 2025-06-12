#pragma once
#include <filesystem>

#include "core/assets/asset_library.hpp"
#include "core/worker/worker_pool.hpp"
#include "core/event/events.hpp"
#include "core/view/view.hpp"

#include "graphics/renderer/renderer_assembler_system.hpp"
#include "graphics/renderer/quad/quad_renderer_system.hpp"
#include "graphics/renderer/instanced/instanced_quad_renderer_system.hpp"

#include "graphics/graphics.hpp"
#include "platform/platform.hpp"

namespace tur
{
	struct TurnipEngine
	{
	public:
		Window window;
		ViewSystem viewSystem;
		AssetLibrary assetLibrary;
		ConfigData configData;
		WorkerPool workerPool;

		GraphicsDevice graphicsDevice;
		tur_unique<GUISystem> guiSystem;

		RendererAssemblerSystem rendererAssemblerSystem;
		QuadRendererSystem quadRendererSystem;
		InstancedQuadSystem instancedQuadSystem;

	public:
		EventCallback mainEventCallback;
		bool shutdownRequested = false;
	};

	void initialize_turnip_engine(TurnipEngine& data, const std::filesystem::path& configPath);
	void turnip_engine_run(TurnipEngine& data);
	void turnip_engine_shutdown(TurnipEngine& data);

	void engine_add_view(TurnipEngine& data, tur_unique<View> view);
	void engine_remove_view(TurnipEngine& data, view_handle textureHandle);
}