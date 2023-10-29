#include "pch.h"
#include <TurnipEngine.h>

using namespace tur;

class MainView : public tur::View
{
public:
	void OnEvent(tur::Event& event)
	{
		Subscriber subscriber(event);

		subscriber.subscribe<WindowCloseEvent>([](const WindowCloseEvent& closeEvent) -> bool {
			TUR_LOG_DEBUG("Window Closed.");
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
		
		// Window:	
		WindowProperties properties;
		{
			properties.windowTitle = "Hello World v1.0";
			properties.dimensions = { 640, 480 };
		};

		RequestWindow(properties);

		// Views:
		AddView(MakeUnique<MainView>());
	}

private:
	void DisplayMonitorInformation()
	{
		uint64_t monitors = Monitor::GetMonitorAmount();
		TUR_LOG_DEBUG("Monitors Available: {}x", monitors);

		for (uint64_t index = 0; index < monitors; ++index)
		{
			MonitorData data = Monitor::FetchMonitorData(index);
			TUR_LOG_DEBUG(" * {} - Size: [{}mm, {}mm] | Content Scale: [{}x, {}x]",
				data.monitorName,
				data.physicalWidth, data.physicalHeight,
				data.contentScaleX, data.contentScaleY
			);

			TUR_LOG_DEBUG(" * Monitor Work Area: [{}, {} - x: {}, y: {}]",
				data.workWidth, data.workHeight, data.workX, data.workY);
		}
		TUR_LOG_DEBUG(' ');
	}

	void DisplayCPUInfo()
	{
		TUR_LOG_DEBUG("Processor Information:");
		DisplayCPUInformation();
	}
};

tur_unique<TurnipEngine> CreateApplication()
{
	return std::make_unique<TurnipEditor>();
}