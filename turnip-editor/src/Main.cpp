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
		return;
		auto& graphics = GraphicsSystem::Get().GetBackend();

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

		subscriber.subscribe<KeyPressedEvent>([](const KeyPressedEvent& keyEvent) -> bool {
			keyEvent.key;
			keyEvent.mods;
			return true;
		});
	}

	void OnRender() override
	{
		glClearColor(0.24f, 0.23f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		GraphicsSystem::Get().GetBackend()->Present();
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

		Graphics().SetupWindow({ "Window Title", { 640, 480 } });
		Graphics().SelectGraphicsBackend(BackendType::OPENGL, {});

		Graphics().SelectGraphicsBackend(BackendType::OPENGL, { 4, 5 });
	
		Graphics().SelectGraphicsBackend(BackendType::VULKAN, { });
		Graphics().InitializeBackend<DefaultVulkanInitializer>();

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