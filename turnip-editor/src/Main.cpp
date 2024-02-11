#include "pch.h"
#include <vulkan/vulkan.h>
#include <TurnipEngine.h>


using namespace tur;

class MainView : public tur::View
{
public:
	void OnEngineInitialize() override
	{
		
	}

	void OnEvent(tur::Event& event) override
	{
		Subscriber subscriber(event);

		subscriber.subscribe<WindowCloseEvent>([](const WindowCloseEvent& closeEvent) -> bool {
			TUR_LOG_DEBUG("Window Closed.");
			return true;
			});

		subscriber.subscribe<KeyPressedEvent>([](const KeyPressedEvent& keyEvent) -> bool {
			keyEvent.key;
			keyEvent.mods;
			return true;
		});
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
		TUR_LOG_INFO(' ');
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