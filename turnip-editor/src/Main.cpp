#include "pch.h"
#include <vulkan/vulkan.h>
#include <TurnipEngine.h>

using namespace tur;

class MainView : public View
{
public:
	void OnEngineStartup() override
	{
		TUR_LOG_INFO("Application initialized");
	}

	void OnEvent(tur::Event& event) override
	{
	}

	void OnRender() override
	{

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
		// System Information:
		{
			DisplayMonitorInformation();

			DisplayCPUInfo();
		}

		// Views:
		View().Add(MakeUnique<MainView>());
	}

private:	
	void DisplayMonitorInformation()
	{
		uint32_t monitorAmount = Monitor::GetMonitorAmount();
		TUR_LOG_INFO("Monitors Available: {}x", monitorAmount);

		for (uint32_t index = 0; index < monitorAmount; ++index)
		{
			MonitorData data = Monitor::FetchMonitorData(index);
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