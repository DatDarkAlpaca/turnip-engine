#include "pch.hpp"
#include "quad_renderer.hpp"

namespace tur
{
	void QuadRenderer::initialize(GraphicsDevice* graphicsDevice)
	{
		m_GraphicsDevice = graphicsDevice;

		m_Commands = tur::make_unique<CommandBuffer>(m_GraphicsDevice->create_command_buffer());
		m_Commands->initialize();

		initialize_pipeline();
		initialize_buffers();
	}

	void QuadRenderer::render()
	{
		m_Commands->begin();
		m_Commands->clear(ClearFlags::COLOR, ClearValue{ m_ClearColor });

		m_Commands->bind_vertex_buffer(buffer, 0);
		m_Commands->bind_index_buffer(indexBuffer);
		{
			m_Commands->bind_pipeline(pipeline);

			for (const auto& quad : m_Quads)
			{
				DataBuffer constantsBuffer;
				constantsBuffer.data = (void*)glm::value_ptr(quad.transform);
				constantsBuffer.size = sizeof(glm::mat4);

				m_Commands->push_constants(0, PipelineStage::ALL, constantsBuffer);
				m_Commands->draw(6, BufferIndexType::UNSIGNED_INT);
			}
		}

		m_Commands->end();
	}

	void QuadRenderer::set_clear_color(const glm::vec4& color)
	{
		m_ClearColor = color;
	}

	void QuadRenderer::add_quad(const QuadData& quad)
	{
		m_Quads.push_back(quad);
	}

	void QuadRenderer::clear_quads()
	{
		m_Quads.clear();
	}

	void QuadRenderer::initialize_pipeline()
	{
		// TODO: allow for different filepaths.
		shader_handle vertexShader = m_GraphicsDevice->create_shader(ShaderDescriptor
			{ "res/shaders/quad.vert", ShaderType::VERTEX
		});
		shader_handle fragmentShader = m_GraphicsDevice->create_shader(ShaderDescriptor
			{ "res/shaders/quad.frag", ShaderType::FRAGMENT
		});

		// Pipeline Layout (Push constants):
		PipelineLayout layout;
		{
			PushConstant constant;
			{
				constant.offset = 0;
				constant.byteSize = sizeof(glm::mat4);
				constant.stages = PipelineStage::ALL;
			}
			layout.add_push_constant(constant);
		}
		
		// Vertex Input:
		VertexInputDescriptor vertexInput;
		{
			BindingDescription bindingDescription;
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = InputRate::VERTEX;
			vertexInput.bindings.push_back(bindingDescription);

			Attribute attribute0;
			attribute0.binding = 0;
			attribute0.location = 0;
			attribute0.format = AttributeFormat::R32G32B32_SFLOAT;
			attribute0.offset = offsetof(QuadRenderer::Vertex, position);
			vertexInput.attributes.push_back(attribute0);

			Attribute attribute1;
			attribute1.binding = 0;
			attribute1.location = 1;
			attribute1.format = AttributeFormat::R32G32_SFLOAT;
			attribute1.offset = offsetof(QuadRenderer::Vertex, uvs);
			vertexInput.attributes.push_back(attribute1);
		}

		PipelineDescriptor descriptor;
		descriptor.vertexInputStage = vertexInput;
		descriptor.fragmentShader = fragmentShader;
		descriptor.vertexShader = vertexShader;
		descriptor.pipelineLayout = layout;

		pipeline = m_GraphicsDevice->create_pipeline(descriptor);
	}

	void QuadRenderer::initialize_buffers()
	{
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
					{{ 0.5f, 0.0f, 0.0f },	{ 1.0f, 0.0f }},
					{{ 0.5f, 0.5f, 0.0f },	{ 1.0f, 1.0f }},
					{{ 0.0f, 0.5f, 0.0f },	{ 0.0f, 1.0f }},
				};
				data.data = vertices;
				data.size = sizeof(vertices);
			}
			buffer = m_GraphicsDevice->create_buffer(bufferDesc, data);
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
			indexBuffer = m_GraphicsDevice->create_buffer(bufferDesc, data);
		}
	}
}