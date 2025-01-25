#include "pch.hpp"
#include "quad_renderer.hpp"

namespace tur
{
	void QuadRenderer::initialize(GraphicsDevice* graphicsDevice, Camera* camera)
	{
		r_GraphicsDevice = graphicsDevice;
		r_Camera = camera;

		m_Commands = tur::make_unique<CommandBuffer>(r_GraphicsDevice->create_command_buffer());
		m_Commands->initialize();

		initialize_pipeline();
		initialize_buffers();
	}

	void QuadRenderer::render()
	{
		m_Commands->begin();
		m_Commands->begin_render();
		m_Commands->set_viewport(m_Viewport);
		/*m_Commands->clear(ClearFlags::COLOR, ClearValue{ m_ClearColor });

		m_Commands->bind_vertex_buffer(buffer, 0);
		m_Commands->bind_index_buffer(indexBuffer);
		m_Commands->bind_pipeline(pipeline);

		for (const auto& quad : m_Quads)
		{
			bind_mvp(quad.transform);

			if (quad.texture != invalid_handle)
				m_Commands->bind_texture(quad.texture);

			else
			{
				if (defaultTexture != invalid_handle)
					m_Commands->bind_texture(defaultTexture);
			}

			m_Commands->draw(6, BufferIndexType::UNSIGNED_INT);
		}*/

		m_Commands->end_render();
		m_Commands->end();
	}

	void QuadRenderer::set_clear_color(const glm::vec4& color)
	{
		m_ClearColor = color;
	}

	void QuadRenderer::set_default_texture(texture_handle handle)
	{
		defaultTexture = handle;
	}

	void QuadRenderer::set_viewport(const Viewport& viewport)
	{
		m_Viewport = viewport;
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
		shader_handle vertexShader = r_GraphicsDevice->create_shader(ShaderDescriptor
			{ "res/shaders/vertex.spv", ShaderType::VERTEX
		});
		shader_handle fragmentShader = r_GraphicsDevice->create_shader(ShaderDescriptor
			{ "res/shaders/fragment.spv", ShaderType::FRAGMENT
		});

		// Pipeline Layout (Push constants):
		PipelineLayout layout;
		{
			DescriptorDescripion description = {};
			{
				description.binding = 0;
				description.stages = PipelineStage::VERTEX_STAGE;
				description.type = BindingTypes::UNIFORM_BUFFER;
				layout.add_binding(description);
			}
			{
				description.binding = 1;
				description.stages = PipelineStage::FRAGMENT_STAGE;
				description.type = BindingTypes::IMAGE_SAMPLER;
				layout.add_binding(description);
			}
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

		pipeline = r_GraphicsDevice->create_pipeline(descriptor);
	}

	void QuadRenderer::initialize_buffers()
	{
		// Vertex Buffer:
		{
			BufferDescriptor bufferDesc = {};
			{
				bufferDesc.type = BufferType::VERTEX_BUFFER;
				bufferDesc.usage = BufferUsage::STATIC;
			}

			DataBuffer data;
			{
				Vertex vertices[4] = {
					{{ -0.5f, -0.5f, 0.0f },	{ 0.0f, 0.0f }},
					{{  0.5f, -0.5f, 0.0f },	{ 1.0f, 0.0f }},
					{{  0.5f,  0.5f, 0.0f },	{ 1.0f, 1.0f }},
					{{ -0.5f,  0.5f, 0.0f },	{ 0.0f, 1.0f }},
				};
				data.data = vertices;
				data.size = sizeof(vertices);
			}
			buffer = r_GraphicsDevice->create_buffer(bufferDesc, data);
		}

		// Index:
		{
			BufferDescriptor bufferDesc = {};
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
			indexBuffer = r_GraphicsDevice->create_buffer(bufferDesc, data);
		}

		// Uniform Buffer:
		{
			BufferDescriptor bufferDesc = {};
			{
				bufferDesc.type = BufferType::UNIFORM_BUFFER;
				bufferDesc.usage = BufferUsage::DYNAMIC;
			}
			uniformBuffer = r_GraphicsDevice->create_buffer(bufferDesc, sizeof(glm::mat4) * 3);
		}
	}

	void QuadRenderer::bind_mvp(const glm::mat4& transform)
	{
		glm::mat4 matrices[3] = {
			transform,
			r_Camera->view(),
			r_Camera->projection()
		};

		DataBuffer dataBuffer;
		dataBuffer.size = sizeof(matrices);
		dataBuffer.data = matrices;

		m_Commands->update_buffer(uniformBuffer, 0, dataBuffer);
		m_Commands->bind_descriptors(uniformBuffer, 0);
	}
}