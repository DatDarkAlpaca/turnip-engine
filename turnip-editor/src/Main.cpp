#include "pch.hpp"
#include <TurnipEngine.hpp>
#include "Core/Assets/TextureLoader.hpp"
#include "Core/Worker/WorkerPool.hpp"

#include <iostream>

using namespace tur;

class MainView : public View
{
public:
	void OnEngineStartup() override
	{
		TUR_LOG_INFO("Application initialized");

		WorkerPool().SubmitTask(TextureLoader::Load, std::tie("mario_thick_ass.png"), [&](TextureAsset asset) {
			TUR_LOG_INFO("Finished loading: {}", asset.metadata.filepath.string());
			AssetLibrary().InsertTexture(asset);
		});
	}

	void OnEngineShutdown() override
	{
		TUR_LOG_INFO("Application shutdown");
	}
};

class TurnipEditor : public TurnipEngine
{
public:
	TurnipEditor()
	{
		AddView(MakeUnique<MainView>());

		// System Information:
		{
			DisplayMonitorInformation();

			DisplayCPUInfo();
		}
	}

private:	
	void DisplayMonitorInformation()
	{
		uint32_t monitorAmount = platform::Monitor::GetMonitorAmount();
		TUR_LOG_INFO("Monitors Available: {}x", monitorAmount);

		for (uint32_t index = 0; index < monitorAmount; ++index)
		{
			MonitorData data = platform::Monitor::FetchMonitorData(index);
			TUR_LOG_INFO(" * {} - Size: [{}mm, {}mm] | Content Scale: [{}x, {}x]",
				data.monitorName,
				data.physicalWidth, data.physicalHeight,
				data.contentScaleX, data.contentScaleY
			);

			TUR_LOG_INFO(" * Monitor Work Area: [{}, {} - x: {}, y: {}]",
				data.workWidth, data.workHeight, data.workX, data.workY);
		}
	}

	void DisplayCPUInfo()
	{
		TUR_LOG_DEBUG("Processor Information:");
		DisplayCPUInformation();
	}
};

tur_unique<TurnipEngine> CreateApplication()
{
	return tur::MakeUnique<TurnipEditor>();
}