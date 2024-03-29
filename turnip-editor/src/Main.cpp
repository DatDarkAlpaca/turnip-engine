#include "pch.h"
#include <TurnipEngine.h>

using namespace tur;

class MainView : public View
{
public:
	MainView(NON_OWNING TurnipEngine* engine)
		: r_Engine(engine)
	{ 
		graphics = r_Engine->Device()->CreateGraphicsCommands();
	}

public:
	void OnEngineStartup() override
	{
		TUR_LOG_INFO("Application initialized");

		auto& device = r_Engine->Device();

		r_Engine->GetWindow().Show();

		// VBO:
		{
			float data[] = {
				0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
				0.5f, 0.0f, 0.0f,      1.0f, 0.0f,
				0.5f, 0.5f, 0.0f,      1.0f, 1.0f,
				0.0f, 0.5f, 0.0f,      0.0f, 1.0f,
			};

			BufferDescriptor bufferDesc = {};
			{
				auto a = UsageFlag::VERTEX_BUFFER;
				auto b = UsageFlag::TRANSFER_DST;
				auto c = a | b;

				bufferDesc.usageFlags = UsageFlag::VERTEX_BUFFER;
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
				bufferDesc.usageFlags = UsageFlag::INDEX_BUFFER;
				bufferDesc.dataSize = sizeof(data);
				bufferDesc.data = data;
			}

			ebo = device->CreateBuffer(bufferDesc);
		}

		// Pipeline:
		{
			// TODO: cross-compiling | transcompiling shaders.
			ShaderDescriptor shaderDesc[2];

			if (r_Engine->Device()->API() == GraphicsAPI::VULKAN)
			{
				shaderDesc[0] = ShaderDescriptor{ "res/shaders/vertex.spv", ShaderType::VERTEX };
				shaderDesc[1] = ShaderDescriptor{ "res/shaders/fragment.spv", ShaderType::FRAGMENT };
			}
			else if (r_Engine->Device()->API() == GraphicsAPI::OPENGL)
			{
				shaderDesc[0] = ShaderDescriptor{ "res/shaders/basic_opengl.vert", ShaderType::VERTEX };
				shaderDesc[1] = ShaderDescriptor{ "res/shaders/basic_opengl.frag", ShaderType::FRAGMENT };
			}
			auto vertexShader = device->CreateShader(shaderDesc[0]);
			auto fragShader = device->CreateShader(shaderDesc[1]);

			VertexFormat vertexFormat;
			vertexFormat.stride = 5 * sizeof(float);
			vertexFormat.Add(VertexAttribute{ 0, Format::R32G32B32_SFLOAT, 0 * sizeof(float) });
			vertexFormat.Add(VertexAttribute{ 1, Format::R32G32_SFLOAT   , 3 * sizeof(float) });

			PipelineStateDescriptor pipelineDesc;
			{
				pipelineDesc.vertexShader = vertexShader;
				pipelineDesc.fragmentShader = fragShader;
				pipelineDesc.frontFace = FrontFace::CLOCKWISE;
				pipelineDesc.cullMode = CullMode::NONE;
				pipelineDesc.vertexFormat = vertexFormat;
			}
			pso = device->CreatePipeline(pipelineDesc);
		}
	}

	void OnEvent(Event& event) override
	{
		Subscriber subscriber(event);
		subscriber.subscribe<KeyPressedEvent>([&](const KeyPressedEvent& keyPress) -> bool {
			if (keyPress.key == Key::KEY_O && r_Engine->Device()->API() == GraphicsAPI::VULKAN)
			{
				r_Engine->ConfigureRenderer({ GraphicsAPI::OPENGL, 3, 3 });

				graphics.reset();
				graphics = r_Engine->Device()->CreateGraphicsCommands();
				OnEngineStartup();
			}
			else if (keyPress.key == Key::KEY_V && r_Engine->Device()->API() == GraphicsAPI::OPENGL)
			{
				r_Engine->ConfigureRenderer({ GraphicsAPI::VULKAN, 1, 1 });
			
				graphics.reset();
				graphics = r_Engine->Device()->CreateGraphicsCommands();
				OnEngineStartup();
			}

			return false;
		});
	}

	void OnRender() override
	{
		auto& device = r_Engine->Device();

		graphics->SetClearColor({ 154.f / 255.f, 230.f / 255.f, 243.f / 255.f, 1.f });
		graphics->Begin();
		{
			graphics->BeginRenderpass();
			graphics->SetViewport();

			graphics->Clear(ClearFlags::COLOR);

			graphics->SetVertexBuffer(vbo);
			graphics->SetIndexBuffer(ebo);

			graphics->BindPipeline(pso);

			graphics->DrawIndexed(6);

			graphics->EndRenderpass();
		}
		graphics->End();

		device->Submit(graphics.get());
		device->Present();
	}

	void OnEngineShutdown() override
	{
		TUR_LOG_INFO("Application shutdown");
	}

private:
	tur_unique<GraphicsRenderCommands> graphics;

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
		ConfigureRenderer({ GraphicsAPI::OPENGL, 3, 3 });

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
		Device()->DisplayVersion();
	}
};

tur_unique<TurnipEngine> CreateApplication()
{
	return tur::MakeUnique<TurnipEditor>();
}