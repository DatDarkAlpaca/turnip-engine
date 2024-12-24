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

		WorkerPool().SubmitTask(TextureLoader::Load, std::tie("mario.png"), [&](TextureAsset asset) {
			TUR_LOG_INFO("Finished loading: {}", asset.metadata.filepath.string());
			AssetLibrary().InsertTexture(asset);
		});

		InitializeGraphics();
	}

	void OnEvent(Event& event) override
	{
		Subscriber subscriber(event);
		subscriber.subscribe<WindowResizeEvent>([&](const WindowResizeEvent& event) {
			swapchain->GetDescriptor().resolution = { 0, 0, (float)event.width, (float)event.height };
			return false;
		});
	}
		
	void OnRender()
	{
		commands->Begin();
		commands->BindVertexBuffer(vbo, 0);
		commands->BindIndexBuffer(ebo);

		{
			commands->BeginRenderPass();
			
			commands->SetViewport(swapchain->GetResolution()); 
			commands->Clear(ClearFlags::COLOR, { { 0.12f,0.12f,0.12f,1.0f } });

			commands->BindPipeline(pipeline);

			commands->DrawIndexed(6);

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
	void InitializeTextures()
	{
		auto& graphics = engine->GetGraphicsSystem().GraphicsLayer();

		TextureDescriptor descriptor
		{
			2, 2,
			1, 1,
			TextureFormat::RGB8,
			TextureUsage::SAMPLING,
			TextureType::TEXTURE_2D,
		};
		
		int data[] = {
			255,   0, 255,
			  0,   0,   0,
			  0,   0,   0,
			255,   0, 255,
		};

		invalidTexture = graphics->CreateTexture(descriptor, data);
	}

	void InitializeGraphics()
	{
		auto& graphics = engine->GetGraphicsSystem().GraphicsLayer();
		commands = graphics->CreateCommandBuffer();

		auto windowSize = engine->GetWindow().GetProperties().dimensions;
		swapchain = graphics->CreateSwapChain({ 0, 0, windowSize.x, windowSize.y });

		{
			PipelineDescriptor pipelineDesc;

			pipelineDesc.vertexShader = graphics->CreateShader({ "res/shaders/basic_opengl.vert", ShaderType::VERTEX });
			pipelineDesc.fragmentShader = graphics->CreateShader({ "res/shaders/basic_opengl.frag", ShaderType::FRAGMENT });
			pipelineDesc.vertexFormat.add(VertexLayout::Attribute{ 0, 0, 1, DataType::R32G32B32_SFLOAT });
			pipelineDesc.vertexFormat.add(VertexLayout::Attribute{ 0, 1, 1, DataType::R32G32_SFLOAT });
			
			pipeline = graphics->CreatePipeline(pipelineDesc);
		}

		// Quads:
		std::vector<float> data = {
			0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,	1.0f, 1.0f,
			0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		};

		BufferDescriptor descriptor;
		descriptor.usage = BufferUsage::VERTEX_BUFFER;
		descriptor.memoryUsage = MemoryUsage::GPU_EXCLUSIVE;
		descriptor.size = sizeof(float) * data.size();
		vbo = graphics->CreateBuffer(descriptor, data.data());

		int indices[6] = { 0, 1, 2, 2, 3, 0 };

		descriptor.usage = BufferUsage::INDEX_BUFFER;
		descriptor.memoryUsage = MemoryUsage::GPU_EXCLUSIVE;
		descriptor.size = sizeof(int) * 6;
		ebo = graphics->CreateBuffer(descriptor, indices);
	}

public:
	tur_unique<ICommandBuffer> commands;
	tur_unique<ISwapchain> swapchain;

private:
	texture_handle invalidTexture;
	buffer_handle vbo, ebo;
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