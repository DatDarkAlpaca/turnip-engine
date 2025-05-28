#include "pch.hpp"
#include "quad_renderer.hpp"

namespace tur
{
	void QuadRenderer::initialize(const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera)
	{
		r_GraphicsDevice = graphicsDevice;
		r_Camera = camera;

		m_Commands = tur::make_unique<CommandBuffer>(r_GraphicsDevice->create_command_buffer());
		m_Commands->initialize();

		m_QuadRendererInfo = configData.quadRendererInformation;

		initialize_pipeline();
		initialize_buffers();
	}

	void QuadRenderer::on_event(Event& event)
	{
		Subscriber subscriber(event);
		subscriber.subscribe<WindowResizeEvent>([&](const WindowResizeEvent& event) -> bool {
			if (renderTarget == invalid_handle)
				return false;

			if (event.width == 0 || event.height == 0)
				return false;

			r_GraphicsDevice->resize_render_target(renderTarget, event.width, event.height);
			return false;
		});
	}

	void QuadRenderer::render()
	{
		m_Commands->begin();
		
		m_Commands->begin_render(renderTarget);
		{
			m_Commands->set_viewport(m_Viewport);
			m_Commands->set_scissor(Rect2D{ 0, 0, m_Viewport.width, m_Viewport.height });
			m_Commands->clear(ClearFlags::COLOR, ClearValue { m_ClearColor });

			m_Commands->bind_vertex_buffer(buffer, 0, sizeof(Vertex));
			m_Commands->bind_index_buffer(indexBuffer);
			m_Commands->bind_pipeline(pipeline);

			m_Commands->set_descriptor_resource(uniformBuffer, DescriptorType::UNIFORM_BUFFER, 0);

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

				m_Commands->draw_indexed(6);
			}
		}
		m_Commands->end_render();
		
		m_Commands->end();
		m_Commands->submit();
	}

	void QuadRenderer::set_camera(Camera* camera)
	{
		r_Camera = camera;
	}

	void QuadRenderer::set_clear_color(const Color& color)
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

	void QuadRenderer::set_render_target_texture(texture_handle handle)
	{
		if (renderTarget != invalid_handle)
			r_GraphicsDevice->destroy_render_target(renderTarget);

		auto textDesc = r_GraphicsDevice->get_textures().get(handle).descriptor;
		RenderTargetDescriptor descriptor = {};
		{
			descriptor.colorAttachments.push_back(handle);
			descriptor.width = textDesc.width;
			descriptor.height = textDesc.height;
		}
		renderTarget = r_GraphicsDevice->create_render_target(descriptor);
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
		// Vertex Input:
		VertexInputDescriptor vertexInput;
		{
			BindingDescription bindingDescription;
			{
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(Vertex);
				bindingDescription.inputRate = InputRate::VERTEX;
				vertexInput.bindings.push_back(bindingDescription);
			}

			Attribute attribute0;
			{
				attribute0.binding = 0;
				attribute0.location = 0;
				attribute0.format = AttributeFormat::R32G32B32_SFLOAT;
				attribute0.offset = offsetof(QuadRenderer::Vertex, position);
				vertexInput.attributes.push_back(attribute0);
			}

			Attribute attribute1;
			{
				attribute1.binding = 0;
				attribute1.location = 1;
				attribute1.format = AttributeFormat::R32G32_SFLOAT;
				attribute1.offset = offsetof(QuadRenderer::Vertex, uvs);
				vertexInput.attributes.push_back(attribute1);
			}
		}

		// Rasterizer:
		RasterizerDescriptor rasterizer = {};
		{
			rasterizer.cullMode = CullMode::FRONT;
			rasterizer.frontFace = FrontFace::COUNTER_CLOCKWISE;
		}

		// Pipeline Layout:
		PipelineLayout layout;
		{
			DescriptorDescription description = {};
			{
				description.binding = 0;
				description.stages = PipelineStage::VERTEX_STAGE;
				description.type = DescriptorType::UNIFORM_BUFFER;
				layout.add_binding(description);
			}
			{
				description.binding = 0;
				description.stages = PipelineStage::FRAGMENT_STAGE;
				description.type = DescriptorType::COMBINED_IMAGE_SAMPLER;
				layout.add_binding(description);
			}
		}

		// Shaders:
		shader_handle vertexShader = r_GraphicsDevice->create_shader(ShaderDescriptor {
			m_QuadRendererInfo.vertexFilepath,
			ShaderType::VERTEX
		});
		shader_handle fragmentShader = r_GraphicsDevice->create_shader(ShaderDescriptor {
			m_QuadRendererInfo.fragmentFilepath,
			ShaderType::FRAGMENT
		});

		PipelineDescriptor descriptor;
		descriptor.vertexInputStage = vertexInput;
		descriptor.fragmentShader = fragmentShader;
		descriptor.vertexShader = vertexShader;
		descriptor.pipelineLayout = layout;
		descriptor.rasterizerStage = rasterizer;

		pipeline = r_GraphicsDevice->create_graphics_pipeline(descriptor);
	}

	void QuadRenderer::initialize_buffers()
	{
		// Vertex Buffer:
		{
			BufferDescriptor bufferDesc = {};
			{
				bufferDesc.type = BufferType::VERTEX_BUFFER;
			}

			DataBuffer data;
			Vertex vertices[4] = {
				{{ -0.5f, -0.5f, 0.0f },	{ 0.0f, 0.0f }},
				{{  0.5f, -0.5f, 0.0f },	{ 1.0f, 0.0f }},
				{{  0.5f,  0.5f, 0.0f },	{ 1.0f, 1.0f }},
				{{ -0.5f,  0.5f, 0.0f },	{ 0.0f, 1.0f }},
			};
			data.data = vertices;
			data.size = sizeof(vertices);

			buffer = r_GraphicsDevice->create_default_buffer(bufferDesc, data);
		}

		// Index:
		{
			BufferDescriptor bufferDesc = {};
			{
				bufferDesc.type = BufferType::INDEX_BUFFER;
			}

			DataBuffer data;
			unsigned int vertices[] = { 0, 1, 2, 2, 3, 0 };
			data.data = vertices;
			data.size = sizeof(vertices);

			indexBuffer = r_GraphicsDevice->create_default_buffer(bufferDesc, data);
		}

		// Uniform Buffer:
		{
			BufferDescriptor bufferDesc = {};
			{
				bufferDesc.type = BufferType::UNIFORM_BUFFER;
				bufferDesc.usage = BufferUsage::DYNAMIC;
			}

			uniformBuffer = r_GraphicsDevice->create_buffer(bufferDesc, sizeof(UBO));
		}
	}

	void QuadRenderer::bind_mvp(const glm::mat4& transform)
	{
		UBO ubo { transform, r_Camera->view(), r_Camera->projection() };

		DataBuffer dataBuffer;
		dataBuffer.size = sizeof(ubo);
		dataBuffer.data = &ubo;

		r_GraphicsDevice->update_buffer(uniformBuffer, dataBuffer);
	}
}