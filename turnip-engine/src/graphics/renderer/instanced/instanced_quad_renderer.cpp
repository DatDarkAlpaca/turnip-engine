#include "pch.hpp"
#include "instanced_quad_renderer.hpp"

namespace tur::instanced_renderer
{
	static void initialize_pipeline(InstancedQuadRenderer& renderer)
	{
		// Vertex Input:
		VertexInputDescriptor vertexInput;
		{
			BindingDescription bindingDescription0;
			{
				bindingDescription0.binding = 0;
				bindingDescription0.stride = sizeof(InstancedQuadRenderer::Vertex);
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
			rasterizer.cullMode = CullMode::FRONT;
			rasterizer.frontFace = FrontFace::COUNTER_CLOCKWISE;
		}

		// Shaders:
		shader_handle vertexShader = renderer.graphicsDevice->create_shader(ShaderDescriptor{
			renderer.info.vertexFilepath,
			ShaderType::VERTEX
		});

		shader_handle fragmentShader = renderer.graphicsDevice->create_shader(ShaderDescriptor{
			renderer.info.fragmentFilepath,
			ShaderType::FRAGMENT
		});

		// Descriptor Layout:
		DescriptorSetLayoutDescriptor descriptorDescriptor;
		{
			DescriptorSetLayoutEntry entry0;
			{
				entry0.binding = 0;
				entry0.type = DescriptorType::COMBINED_IMAGE_SAMPLER;
				entry0.stage = PipelineStage::FRAGMENT_STAGE;
			}
			descriptorDescriptor.push_back(entry0);

			DescriptorSetLayoutEntry entry1;
			{
				entry1.binding = 1;
				entry1.type = DescriptorType::STORAGE_BUFFER;
				entry1.stage = PipelineStage::VERTEX_STAGE;
			}
			descriptorDescriptor.push_back(entry1);
		}

		renderer.descriptor = renderer.graphicsDevice->create_descriptors(descriptorDescriptor);

		// Pipeline:
		PipelineDescriptor descriptor;
		descriptor.vertexInputStage = vertexInput;
		descriptor.fragmentShader = fragmentShader;
		descriptor.vertexShader = vertexShader;
		descriptor.rasterizerStage = rasterizer;
		descriptor.descriptorSetLayouts.push_back(renderer.descriptor);

		renderer.pipeline = renderer.graphicsDevice->create_graphics_pipeline(descriptor);
	}

	static void initialize_buffers(InstancedQuadRenderer& renderer)
	{
		// Vertex Buffer:
		{
			BufferDescriptor bufferDesc = {};
			{
				bufferDesc.type = BufferType::VERTEX_BUFFER;
			}

			DataBuffer data;
			InstancedQuadRenderer::Vertex vertices[4] = {
				{{ -0.5f, -0.5f, 0.0f },	{ 0.0f, 0.0f }},
				{{  0.5f, -0.5f, 0.0f },	{ 1.0f, 0.0f }},
				{{  0.5f,  0.5f, 0.0f },	{ 1.0f, 1.0f }},
				{{ -0.5f,  0.5f, 0.0f },	{ 0.0f, 1.0f }},
			};
			data.data = vertices;
			data.size = sizeof(vertices);

			renderer.vertexBuffer = renderer.graphicsDevice->create_default_buffer(bufferDesc, data);
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

			renderer.indexBuffer = renderer.graphicsDevice->create_default_buffer(bufferDesc, data);
		}

		// VP (View & Projection) Uniform Buffer:
		{
			BufferDescriptor bufferDesc = {};
			{
				bufferDesc.type = BufferType::UNIFORM_BUFFER;
				bufferDesc.usage = BufferUsage::DYNAMIC;
			}

			InstancedQuadRenderer::ViewProjUBO uboData = {};
			if (renderer.camera)
			{
				uboData.projection = renderer.camera->projection();
				uboData.view = renderer.camera->view();
			}

			DataBuffer data;
			data.data = &uboData;
			data.size = sizeof(InstancedQuadRenderer::ViewProjUBO);
			renderer.viewProjBuffer = renderer.graphicsDevice->create_default_buffer(bufferDesc, data);
		}

		// Instance Buffer:
		{
			BufferDescriptor bufferDesc = {};
			{
				bufferDesc.type = BufferType::STORAGE_BUFFER;
				bufferDesc.usage = BufferUsage::DYNAMIC | BufferUsage::PERSISTENT | BufferUsage::COHERENT | BufferUsage::WRITE;
			}

			u32 instanceBufferSize = sizeof(InstanceData) * renderer.info.maxInstanceCount;
			renderer.instanceBuffer = renderer.graphicsDevice->create_buffer(bufferDesc, sizeof(InstanceData) * renderer.info.maxInstanceCount);

			renderer.instanceMappedData = renderer.graphicsDevice->map_buffer(
				renderer.instanceBuffer, 0, instanceBufferSize,
				AccessFlags::PERSISTENT | AccessFlags::WRITE | AccessFlags::COHERENT
			);
		}

		renderer.graphicsDevice->update_descriptor_resource(renderer.pipeline, renderer.viewProjBuffer, DescriptorType::UNIFORM_BUFFER, 0);
		renderer.graphicsDevice->update_descriptor_resource(renderer.pipeline, renderer.instanceBuffer, DescriptorType::STORAGE_BUFFER, 1);
	}

	static void initialize_textures(InstancedQuadRenderer& renderer)
	{
		TextureDescriptor descriptor;
		{
			descriptor.type = TextureType::ARRAY_TEXTURE_2D;
			descriptor.width = renderer.info.textureWidth;
			descriptor.height = renderer.info.textureHeight;
			descriptor.depth = std::max<u32>(1, renderer.info.maxTextureCount);
		}
		renderer.textureArray = renderer.graphicsDevice->create_texture(descriptor);
	}
}

namespace tur
{
	void initialize_instanced_quad_renderer(InstancedQuadRenderer& renderer, const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera)
	{
		renderer.graphicsDevice = graphicsDevice;
		renderer.camera = camera;

		renderer.commands = tur::make_unique<CommandBuffer>(renderer.graphicsDevice->create_command_buffer());
		renderer.commands->initialize();

		renderer.info = configData.instancedQuadRendererInformation;
		
		instanced_renderer::initialize_pipeline(renderer);
		instanced_renderer::initialize_buffers(renderer);
		instanced_renderer::initialize_textures(renderer);
	}

	void instanced_quad_renderer_begin(InstancedQuadRenderer& renderer, render_target_handle handle)
	{
		auto& commands = renderer.commands;
		commands->begin_render(handle);
	}

	void instanced_quad_renderer_render(InstancedQuadRenderer& renderer)
	{
		auto& commands = renderer.commands;

		commands->set_viewport(renderer.viewport);
		commands->set_scissor(Rect2D{ 0, 0, renderer.viewport.width, renderer.viewport.height });
		
		if(renderer.shouldClear)
			commands->clear(ClearFlags::COLOR, ClearValue{ renderer.clearColor });

		commands->bind_vertex_buffer(renderer.vertexBuffer, 0, sizeof(InstancedQuadRenderer::Vertex));
		commands->bind_index_buffer(renderer.indexBuffer);
		commands->bind_pipeline(renderer.pipeline);

		commands->bind_texture(renderer.textureArray, 0);
		commands->draw_indexed(6, renderer.quadAmount);
	}

	void instanced_quad_renderer_end(InstancedQuadRenderer& renderer)
	{
		auto& commands = renderer.commands;
		commands->end_render();
	}
}

namespace tur
{
	u32 instanced_quad_renderer_add_quad(InstancedQuadRenderer& renderer, const InstanceData& quadData)
	{
		u32 index = renderer.quadAmount;

		InstanceData* data = (InstanceData*)renderer.instanceMappedData;
		data[index] = quadData;

		++renderer.quadAmount;
		return index;
	}

	void instanced_quad_renderer_update_quad(InstancedQuadRenderer& renderer, const InstanceData& quadData, u32 index)
	{
		TUR_ASSERT(renderer.info.maxInstanceCount > index, "Invalid instanced renderer index");
	
		InstanceData* data = (InstanceData*)renderer.instanceMappedData;
		data[index] = quadData;
	}

	void instanced_quad_renderer_add_texture(InstancedQuadRenderer& renderer, const TextureAsset& asset)
	{
		TextureAsset modifiedAsset = asset;
		modifiedAsset.zOffset = renderer.textureCount;
		modifiedAsset.width = renderer.info.textureWidth;
		modifiedAsset.height = renderer.info.textureHeight;
		modifiedAsset.depth = 1;

		renderer.graphicsDevice->update_texture(renderer.textureArray, modifiedAsset);
		++renderer.textureCount;
	}
}