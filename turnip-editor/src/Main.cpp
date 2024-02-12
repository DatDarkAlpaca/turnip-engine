#include "pch.h"
#include <vulkan/vulkan.h>
#include <TurnipEngine.h>

using namespace tur;

class MainView : public View
{
public:
	MainView(NON_OWNING TurnipEngine* engine)
		: r_Engine(engine)
	{

	}

public:
	void OnEngineStartup() override
	{
		TUR_LOG_INFO("Application initialized");
		glClearColor(154.f / 255.f, 230.f / 255.f, 243.f / 255.f, 1.f);
	}

	void OnRender() override
	{
		// renderer.setViewport()
		// renderer.set
		// renderer.submit(whatever);

		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(std::any_cast<GLFWwindow*>(r_Engine->GetWindow().GetHandle()));
	}

	void OnEngineShutdown() override
	{
		TUR_LOG_INFO("Application shutdown");
	}

private:
	// renderer
	NON_OWNING TurnipEngine* r_Engine = nullptr;
};

class TurnipEditor : public TurnipEngine
{
public:
	TurnipEditor()
	{
		// Rendering options:
		ConfigureRenderer({ GraphicsAPI::OPENGL, 4, 5 });

		// System Information:
		{
			DisplayMonitorInformation();

			DisplayCPUInfo();

			DisplayRenderInfo();
		}

		// Views:
		View().Add(MakeUnique<MainView>(this));
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

	void DisplayRenderInfo()
	{
		TUR_LOG_DEBUG("Graphics API Information:");
		Renderer().DisplayVersion();
	}
};

tur_unique<TurnipEngine> CreateApplication()
{
	return tur::MakeUnique<TurnipEditor>();
}