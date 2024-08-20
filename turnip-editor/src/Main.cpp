#include "pch.hpp"
#include <TurnipEngine.hpp>
#include "Core/Assets/TextureLoader.hpp"

using namespace tur;

class MainView : public View
{
public:
	void OnEngineStartup() override
	{
		TUR_LOG_INFO("Application initialized");

	/*	WorkerPool().SubmitTask(TextureLoader::Load, std::tie("mario_thick_ass.png"), [&](TextureAsset asset) {
			TUR_LOG_INFO("Finished loading: {}", asset.metadata.filepath.string());
			AssetLibrary().InsertTexture(asset);
		});*/

		InitializeGraphics();
	}
		
	void OnRender()
	{
		commands->Begin();
		commands->BindVertexBuffer(vbo, 0);

		{
			commands->BeginRenderPass();
			commands->Clear(ClearFlags::COLOR, { { 0.12f,0.12f,0.12f,1.0f } });

			commands->BindPipeline(pipeline);

			commands->Draw(0, 3);

			commands->EndRenderPass();
		}
		
		commands->End();
		commands->Submit();

		swapchain->Present();
	}

	void OnRenderGUI() override
	{
		/*ImGui::Begin("Frame");
		ImGui::End();*/
	}

	void OnEngineShutdown() override
	{
		TUR_LOG_INFO("Application shutdown");
	}

private:
	void InitializeGraphics()
	{
		auto& graphics = engine->GetGraphicsSystem().GraphicsLayer();
		commands = graphics->CreateCommandBuffer();
		swapchain = graphics->CreateSwapChain({ {800, 600} });

		{
			PipelineDescriptor pipelineDesc;

			pipelineDesc.vertexShader = graphics->CreateShader({ "res/shaders/basic_opengl.vert", ShaderType::VERTEX });
			pipelineDesc.fragmentShader = graphics->CreateShader({ "res/shaders/basic_opengl.frag", ShaderType::FRAGMENT });
			pipelineDesc.vertexFormat.add(VertexFormat::Attribute{ 0, 0, 1, DataType::R32G32B32_SFLOAT });
			pipelineDesc.vertexFormat.add(VertexFormat::Attribute{ 0, 1, 1, DataType::R32G32_SFLOAT });
			
			pipeline = graphics->CreatePipeline(pipelineDesc);
		}

		// Quads:
		std::vector<float> data = {
			0.0f, 0.0f, 0.0f,	1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
			1.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		};

		BufferDescriptor descriptor;
		descriptor.usage = BufferUsage::VERTEX_BUFFER;
		descriptor.memoryUsage = MemoryUsage::GPU_EXCLUSIVE;
		descriptor.size = sizeof(float) * data.size();
		vbo = graphics->CreateBuffer(descriptor, data.data());
	}

public:
	tur_unique<ICommandBuffer> commands;
	tur_unique<ISwapchain> swapchain;

private:
	buffer_handle vbo;
	pipeline_handle pipeline;
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