#include "pch.hpp"
#include "instanced_quad_renderer.hpp"

namespace tur
{
    void InstancedQuadRenderer::initialize(const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera)
    {
		r_GraphicsDevice = graphicsDevice;
		r_Camera = camera;

		m_Commands = tur::make_unique<CommandBuffer>(r_GraphicsDevice->create_command_buffer());
		m_Commands->initialize();

		m_RendererInfo = configData.instancedQuadRendererInformation;

		initialize_pipeline();
		initialize_buffers();
		initialize_textures();
    }
	void InstancedQuadRenderer::render()
	{
		m_Commands->begin();

		m_Commands->begin_render();
		{
			m_Commands->set_viewport(m_Viewport);
			m_Commands->set_scissor(Rect2D{ 0, 0, m_Viewport.width, m_Viewport.height });
			m_Commands->clear(ClearFlags::COLOR, ClearValue{ m_ClearColor });

			m_Commands->bind_vertex_buffer(vertexBuffer, 0, sizeof(Vertex));
			m_Commands->bind_index_buffer(indexBuffer);
			m_Commands->bind_pipeline(pipeline);

			m_Commands->set_descriptor_resource(vpBuffer, DescriptorType::UNIFORM_BUFFER, 0);
			m_Commands->set_descriptor_resource(instanceBuffer, DescriptorType::STORAGE_BUFFER, 1);

			m_Commands->bind_texture(textureArray, 0);
			m_Commands->draw_indexed(6, quadAmount);
		}
		m_Commands->end_render();

		m_Commands->end();
		m_Commands->submit();
	}

	void InstancedQuadRenderer::set_camera(Camera* camera)
	{
		r_Camera = camera;

		VPUBO uboData;
		uboData.projection = r_Camera->projection();
		uboData.view = r_Camera->view();

		DataBuffer data;
		data.data = &uboData;
		data.size = sizeof(VPUBO);

		r_GraphicsDevice->update_buffer(vpBuffer, data);
	}

	void InstancedQuadRenderer::set_clear_color(const glm::vec4& color)
	{
		m_ClearColor = color;
	}
	void InstancedQuadRenderer::set_viewport(const Viewport& viewport)
	{
		m_Viewport = viewport;
	}

	void InstancedQuadRenderer::add_quad(const InstanceData& quadData)
	{
		InstanceData* data = (InstanceData*)instanceMappedData;
		data[quadAmount] = quadData;

		++quadAmount;
	}
	void InstancedQuadRenderer::add_texture(const TextureAsset& asset)
	{
		TextureAsset modifiedAsset = asset;
		modifiedAsset.zOffset = textureCount;
		modifiedAsset.width = m_RendererInfo.textureWidth;
		modifiedAsset.height = m_RendererInfo.textureHeight;
		modifiedAsset.depth = 1;

		r_GraphicsDevice->update_texture(textureArray, modifiedAsset);
		++textureCount;
	}

	void InstancedQuadRenderer::initialize_pipeline()
	{
		// Vertex Input:
		VertexInputDescriptor vertexInput;
		{
			BindingDescription bindingDescription0;
			{
				bindingDescription0.binding = 0;
				bindingDescription0.stride = sizeof(Vertex);
				bindingDescription0.inputRate = InputRate::VERTEX;
				vertexInput.bindings.push_back(bindingDescription0);
			}

			Attribute attribute0;
			{
				attribute0.binding = 0;
				attribute0.location = 0;
				attribute0.format = AttributeFormat::R32G32B32_SFLOAT;
				attribute0.offset = offsetof(InstancedQuadRenderer::Vertex, position);
				vertexInput.attributes.push_back(attribute0);
			}

			Attribute attribute1;
			{
				attribute1.binding = 0;
				attribute1.location = 1;
				attribute1.format = AttributeFormat::R32G32_SFLOAT;
				attribute1.offset = offsetof(InstancedQuadRenderer::Vertex, uvs);
				vertexInput.attributes.push_back(attribute1);
			}
		}

		// Rasterizer:
		RasterizerDescriptor rasterizer = {};
		{
			rasterizer.cullMode  = CullMode::FRONT;
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
				description.binding = 1;
				description.stages = PipelineStage::VERTEX_STAGE;
				description.type = DescriptorType::STORAGE_BUFFER;
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
		shader_handle vertexShader = r_GraphicsDevice->create_shader(ShaderDescriptor{
			m_RendererInfo.vertexFilepath,
			ShaderType::VERTEX
		});
	
		shader_handle fragmentShader = r_GraphicsDevice->create_shader(ShaderDescriptor{
			m_RendererInfo.fragmentFilepath,
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
	void InstancedQuadRenderer::initialize_buffers()
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

			vertexBuffer = r_GraphicsDevice->create_default_buffer(bufferDesc, data);
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
		
		// VP (View & Projection) Uniform Buffer:
		{
			BufferDescriptor bufferDesc = {};
			{
				bufferDesc.type = BufferType::UNIFORM_BUFFER;
				bufferDesc.usage = BufferUsage::DYNAMIC;
			}

			VPUBO uboData;
			if (r_Camera)
			{
				uboData.projection = r_Camera->projection();
				uboData.view = r_Camera->view();
			}

			DataBuffer data;
			data.data = &uboData;
			data.size = sizeof(VPUBO);
			vpBuffer = r_GraphicsDevice->create_default_buffer(bufferDesc, data);
		}
		
		// Instance Buffer:
		{
			BufferDescriptor bufferDesc = {};
			{
				bufferDesc.type = BufferType::STORAGE_BUFFER;
				bufferDesc.usage = BufferUsage::DYNAMIC | BufferUsage::PERSISTENT | BufferUsage::COHERENT | BufferUsage::WRITE;
			}

			u32 instanceBufferSize = sizeof(InstanceData) * m_RendererInfo.maxInstanceCount;
			instanceBuffer = r_GraphicsDevice->create_buffer(bufferDesc, sizeof(InstanceData) * m_RendererInfo.maxInstanceCount);

			instanceMappedData = r_GraphicsDevice->map_buffer(instanceBuffer, 0, instanceBufferSize,
				AccessFlags::PERSISTENT | AccessFlags::WRITE | AccessFlags::COHERENT);
		}
	}
	void InstancedQuadRenderer::initialize_textures()
	{
		TextureDescriptor descriptor;
		{
			descriptor.type = TextureType::ARRAY_TEXTURE_2D;
			descriptor.width = m_RendererInfo.textureWidth;
			descriptor.height = m_RendererInfo.textureHeight;
			descriptor.depth = std::max<u32>(1, m_RendererInfo.maxTextureCount);
		}
		textureArray = r_GraphicsDevice->create_texture(descriptor);
	}
}