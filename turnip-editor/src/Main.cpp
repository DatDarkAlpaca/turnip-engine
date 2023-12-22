#include "pch.h"
#include <vulkan/vulkan.h>
#include <TurnipEngine.h>

#include "Graphics/Vulkan/Objects/Pipeline.h"
#include "Graphics/Vulkan/Builders/FramebufferBuilder.h"

using namespace tur;

class MainView : public tur::View
{
public:
	void OnEngineInitialize() override
	{
		auto* graphics = (BackendVulkan*)GraphicsSystem::Get().GetBackend().get();

		auto vertexShader = graphics->CreateShader({ "res/shaders/vertex.spv", ShaderType::VERTEX });
		auto fragShader = graphics->CreateShader({ "res/shaders/fragment.spv", ShaderType::FRAGMENT });

		PipelineDescriptor descriptor;
		{
			descriptor.vertexShader = vertexShader.get();
			descriptor.fragmentShader = fragShader.get();
		};

		pipeline = graphics->CreatePipeline(descriptor);
	}

	void OnEvent(tur::Event& event) override
	{
		Subscriber subscriber(event);

		subscriber.subscribe<WindowCloseEvent>([](const WindowCloseEvent& closeEvent) -> bool {
			TUR_LOG_DEBUG("Window Closed.");
			return true;
		});
	}

private:
	tur_unique<Pipeline> pipeline;
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

		Graphics().SetupWindow({ "Turnip Engine v1.0", { 640, 480 } });

		Graphics().SelectGraphicsBackend(BackendType::VULKAN, { });
		Graphics().InitializeBackend<DefaultVulkanInitializer>();

		// Views:
		View().Add(MakeUnique<MainView>());
	}

private:	
	void DisplayMonitorInformation()
	{
		uint64_t monitors = Monitor::GetMonitorAmount();
		TUR_LOG_INFO("Monitors Available: {}x", monitors);

		for (uint64_t index = 0; index < monitors; ++index)
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