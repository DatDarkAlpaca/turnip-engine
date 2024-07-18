#include "pch.h"
#include <TurnipEngine.h>
#include "Core/Assets/TextureLoader.hpp"

using namespace tur;

class MainView : public View
{
public:
	void OnEngineStartup() override
	{
		TUR_LOG_INFO("Application initialized");

		// Texture loading:
		auto textureAsset = TextureLoader::Load({ "mario_thick_ass.png" });
		AssetLibrary().InsertTexture(textureAsset);
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