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

		auto& device = r_Engine->Device();

		// Pipeline:
		{
			ShaderDescriptor shaderDesc[2];
			{
				shaderDesc[0] = ShaderDescriptor{ "res/shaders/basic.vert", ShaderType::VERTEX };
				shaderDesc[1] = ShaderDescriptor{ "res/shaders/basic.frag", ShaderType::FRAGMENT };
			}
			auto vertexShader = device->CreateShader(shaderDesc[0]);
			auto fragShader = device->CreateShader(shaderDesc[1]);

			PipelineStateDescriptor pipelineDesc;
			{
				pipelineDesc.vertexShader = vertexShader;
				pipelineDesc.fragmentShader = fragShader;
			}
			pso = device->CreatePipeline(pipelineDesc);
		}

		glGenVertexArrays(1, &vao);
		//glBindVertexArray(vao);

		// VBO:
		{
			float data[] = {
				0.0f, 0.0f, 0.0f,
				0.5f, 0.0f, 0.0f,
				0.5f, 0.5f, 0.0f,
			};

			BufferDescriptor bufferDesc;
			{
				bufferDesc.bindingFlag = BindingFlag::ARRAY_BUFFER;
				bufferDesc.usageFlag = UsageFlag::STATIC_DRAW;
				bufferDesc.dataSize = sizeof(data);
				bufferDesc.data = data;
			}

			vbo = device->CreateBuffer(bufferDesc);
		}
		return;
		// EBO:
		{
			unsigned int data[] = { 0, 1, 2	};

			BufferDescriptor bufferDesc;
			{
				bufferDesc.bindingFlag = BindingFlag::ARRAY_BUFFER;
				bufferDesc.usageFlag = UsageFlag::STATIC_DRAW;
				bufferDesc.dataSize = sizeof(data);
				bufferDesc.data = data;
			}

			ebo = device->CreateBuffer(bufferDesc);
		}
	}

	void OnRender() override
	{
		auto* device = r_Engine->Device().get();

		glClear(GL_COLOR_BUFFER_BIT);
		auto id = (static_cast<RenderDeviceGL*>(device))->GetPipeline(pso).id;
		glUseProgram(id);

		glBindVertexArray(vao);
		
		auto vboid = (static_cast<RenderDeviceGL*>(device))->GetBuffer(vbo).id;
		glBindBuffer(GL_ARRAY_BUFFER, vboid);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(std::any_cast<GLFWwindow*>(r_Engine->GetWindow().GetHandle()));
	}

	void OnEngineShutdown() override
	{
		TUR_LOG_INFO("Application shutdown");
	}

private:
	uint32_t vao;
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