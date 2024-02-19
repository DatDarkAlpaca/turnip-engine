#include "pch.h"
#include <vulkan/vulkan.h>
#include <TurnipEngine.h>
#include "Graphics/Vulkan/VulkanInitializer.h"

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

		auto& graphics = r_Engine->GraphicsContext();
		auto& device = r_Engine->Device();
		
		// Pipeline:
		{
			ShaderDescriptor shaderDesc[2];
			{
				shaderDesc[0] = ShaderDescriptor{ "res/shaders/vertex.spv", ShaderType::VERTEX };
				shaderDesc[1] = ShaderDescriptor{ "res/shaders/fragment.spv", ShaderType::FRAGMENT };
			}
			auto vertexShader = device->CreateShader(shaderDesc[0]);
			auto fragShader = device->CreateShader(shaderDesc[1]);

			PipelineStateDescriptor pipelineDesc;
			{
				pipelineDesc.vertexShader = vertexShader;
				pipelineDesc.fragmentShader = fragShader;
				pipelineDesc.frontFace = FrontFace::COUNTER_CLOCKWISE;
				pipelineDesc.primitiveTopology = PrimitiveTopology::TRIANGLES;
				pipelineDesc.inputLayouts.push_back(InputLayoutElement{ 0, 3, LayoutType::FLOAT_32, false });
				pipelineDesc.inputLayouts.push_back(InputLayoutElement{ 1, 2, LayoutType::FLOAT_32, false });
			}
			pso = device->CreatePipeline(pipelineDesc);
		}

		// VBO:
		{
			float data[] = {
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
				0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
			};

			BufferDescriptor bufferDesc = {};
			{
				bufferDesc.bindingFlag = BindingFlag::ARRAY_BUFFER;
				bufferDesc.usageFlag = UsageFlag::STATIC_DRAW;
				bufferDesc.dataSize = sizeof(data);
				bufferDesc.data = data;
			}

			vbo = device->CreateBuffer(bufferDesc);
		}

		// EBO:
		{
			unsigned int data[] = { 0, 1, 2, 2, 3, 0 };

			BufferDescriptor bufferDesc = {};
			{
				bufferDesc.bindingFlag = BindingFlag::ELEMENT_ARRAY_BUFFER;
				bufferDesc.usageFlag = UsageFlag::STATIC_DRAW;
				bufferDesc.dataSize = sizeof(data);
				bufferDesc.data = data;
			}

			ebo = device->CreateBuffer(bufferDesc);
		}

		device->FinishSetup();

		// Initialization:
		graphics->SetClearColor({ 154.f / 255.f, 230.f / 255.f, 243.f / 255.f, 1.f });
	}

	void OnRender() override
	{
		auto& graphics = r_Engine->GraphicsContext();
		auto& device = r_Engine->Device();

		graphics->Begin();
		{
			graphics->Clear();

			graphics->SetVertexBuffer(vbo);
			graphics->SetIndexBuffer(ebo);

			graphics->SetPipeline(pso);

			graphics->DrawIndexed(6);
		}
		graphics->End();

		// ImGUI:
		{
			/*r_Engine->Renderer().BeginFrame();

			ImGui::Begin("Hello");
			ImGui::End();

			r_Engine->Renderer().EndFrame();*/
		}
		
		device->Present();
	}

	void OnEngineShutdown() override
	{
		TUR_LOG_INFO("Application shutdown");
	}

private:
	BufferHandle vbo = BufferHandle::INVALID;
	BufferHandle ebo = BufferHandle::INVALID;
	PipelineStateHandle pso = PipelineStateHandle::INVALID;

	// renderer
	NON_OWNING TurnipEngine* r_Engine = nullptr;
};

class TurnipEditor : public TurnipEngine
{
public:
	TurnipEditor()
	{
		// Rendering options:
		ConfigureRenderer({ GraphicsAPI::VULKAN, 1, 0 });
		vulkan::DefaultVulkanInitializer initializer { static_cast<vulkan::RenderDeviceVK*>(Device().get()) };

		// Views:
		View().Add(MakeUnique<MainView>(this));

		// System Information:
		{
			DisplayMonitorInformation();

			DisplayCPUInfo();

			DisplayRenderInfo();
		}
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