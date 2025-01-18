#include "pch.hpp"
#include <turnip_engine.hpp>
#include <platform/platform.hpp>

using namespace tur;

struct ApplicationContext
{
	Window window;
	ViewSystem viewSystem;
	AssetLibrary assetLibrary;
	WorkerPool workerPool;

	tur_unique<GraphicsDevice> device;
	tur_unique<CommandBuffer> commands;
} context;

// Application:
static void initialize()
{
	using namespace std::chrono_literals;

	// Textures:
	context.workerPool.submit<asset_handle>([&]() {
		std::this_thread::sleep_for(2000ms);
		return load_texture_asset(&context.assetLibrary, "res/textures/face.png");
	}, [&](asset_handle handle) {
		auto filepath = context.assetLibrary.textures.get(handle).filepath;
		TUR_LOG_INFO("Loaded texture successfully: {}", filepath.string());
	});
}

static void on_event(Event& event)
{

}

static void on_update()
{

}

static void initialize_context(ApplicationContext* context)
{
	// Logger:
	initialize_logger_system();

	// Worker Pool:
	context->workerPool.initialize();

	// Window:
	context->window.title = "TurnipEngine v1.0";
	initialize_windowing_system();
	set_callback_window(&context->window, on_event);

	{
		GraphicsSpecification specification;
		specification.major = 4;
		specification.minor = 3;
		specification.api = GraphicsAPI::OPENGL;

		initialize_opengl_windowing(&context->window, specification);
	}
}

int main()
{
	initialize_context(&context);
	initialize();

	// Graphics:
	pipeline_handle pipeline;
	buffer_handle buffer;
	buffer_handle indexBuffer;
	{
		context.device = tur::make_unique<GraphicsDevice>(&context.window);
		context.device->initialize();

		context.commands = tur::make_unique<CommandBuffer>(context.device->create_command_buffer());
		context.commands->initialize();

		shader_handle vert = context.device->create_shader(ShaderDescriptor{ "res/shaders/basic_opengl.vert", ShaderType::VERTEX });
		shader_handle frag = context.device->create_shader(ShaderDescriptor{ "res/shaders/basic_opengl.frag", ShaderType::FRAGMENT });

		struct Vertex
		{
			glm::vec3 position;
			glm::vec2 uvs;
		};

		VertexInputDescriptor vertexInput;
		{
			BindingDescription bindingDescription;
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = InputRate::VERTEX;
			vertexInput.bindings.push_back(bindingDescription);

			Attribute attribute0;
			attribute0.binding  = 0;
			attribute0.location = 0;
			attribute0.format   = AttributeFormat::R32G32B32_SFLOAT;		// vec3
			attribute0.offset   = 0;
			vertexInput.attributes.push_back(attribute0);

			Attribute attribute1;
			attribute1.binding = 0;
			attribute1.location = 1;
			attribute1.format = AttributeFormat::R32G32_SFLOAT;				// vec2
			attribute1.offset = sizeof(float) * 3;
			vertexInput.attributes.push_back(attribute1);
		}
		
		PipelineDescriptor descriptor;
		descriptor.vertexInputStage = vertexInput;
		descriptor.fragmentShader = frag;
		descriptor.vertexShader = vert;

		pipeline = context.device->create_pipeline(descriptor);

		// Buffer:
		{
			BufferDescriptor bufferDesc;
			{
				bufferDesc.type = BufferType::VERTEX_BUFFER;
				bufferDesc.usage = BufferUsage::STATIC;
			}

			DataBuffer data;
			{
				Vertex vertices[4] = {
					{{ 0.0f, 0.0f, 0.0f },	{ 0.0f, 0.0f }},
					{{ 1.0f, 0.0f, 0.0f },	{ 1.0f, 0.0f }},
					{{ 1.0f, 1.0f, 0.0f },	{ 1.0f, 1.0f }},
					{{ 0.0f, 1.0f, 0.0f },	{ 0.0f, 1.0f }},
				};
				data.data = vertices;
				data.size = sizeof(vertices);
			}
			buffer = context.device->create_buffer(bufferDesc, data);
		}

		// Index:
		{
			BufferDescriptor bufferDesc;
			{
				bufferDesc.type = BufferType::INDEX_BUFFER;
				bufferDesc.usage = BufferUsage::STATIC;
			}

			DataBuffer data;
			{
				unsigned int vertices[] = { 0, 1, 2, 2, 3, 0 };
				data.data = vertices;
				data.size = sizeof(vertices);
			}
			indexBuffer = context.device->create_buffer(bufferDesc, data);
		}
	}

	while (is_open_window(&context.window))
	{
		poll_events(&context.window);

		on_update();

		context.commands->begin();
		context.commands->clear(ClearFlags::COLOR, ClearValue{{ 0.16f, 0.16f, 0.16f, 1.f }});

		context.commands->bind_vertex_buffer(buffer, 0);
		context.commands->bind_index_buffer(indexBuffer);
		{
			context.commands->bind_pipeline(pipeline);
			context.commands->draw(6, BufferIndexType::UNSIGNED_INT);
		}

		context.commands->end();

		context.device->present();
	}

	shutdown_window(&context.window);
	shutdown_windowing_system();

	return 0;
}
