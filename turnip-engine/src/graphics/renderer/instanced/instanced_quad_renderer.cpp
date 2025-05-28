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
		shader_handle vertexShader = renderer.graphicsDevice->create_shader(ShaderDescriptor{
			renderer.info.vertexFilepath,
			ShaderType::VERTEX
			});

		shader_handle fragmentShader = renderer.graphicsDevice->create_shader(ShaderDescriptor{
			renderer.info.fragmentFilepath,
			ShaderType::FRAGMENT
			});

		PipelineDescriptor descriptor;
		descriptor.vertexInputStage = vertexInput;
		descriptor.fragmentShader = fragmentShader;
		descriptor.vertexShader = vertexShader;
		descriptor.pipelineLayout = layout;
		descriptor.rasterizerStage = rasterizer;

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

	void instanced_quad_renderer_render(InstancedQuadRenderer& renderer)
	{
		auto& commands = renderer.commands;
		commands->begin();

		commands->begin_render();
		{
			commands->set_viewport(renderer.viewport);
			commands->set_scissor(Rect2D{ 0, 0, renderer.viewport.width, renderer.viewport.height });
			commands->clear(ClearFlags::COLOR, ClearValue{ renderer.clearColor });

			commands->bind_vertex_buffer(renderer.vertexBuffer, 0, sizeof(InstancedQuadRenderer::Vertex));
			commands->bind_index_buffer(renderer.indexBuffer);
			commands->bind_pipeline(renderer.pipeline);

			commands->set_descriptor_resource(renderer.viewProjBuffer, DescriptorType::UNIFORM_BUFFER, 0);
			commands->set_descriptor_resource(renderer.instanceBuffer, DescriptorType::STORAGE_BUFFER, 1);

			commands->bind_texture(renderer.textureArray, 0);
			commands->draw_indexed(6, renderer.quadAmount);
		}
		commands->end_render();

		commands->end();
		commands->submit();
	}
}

namespace tur
{
	void instanced_quad_renderer_set_camera(InstancedQuadRenderer& renderer, Camera* camera)
	{
		renderer.camera = camera;

		InstancedQuadRenderer::ViewProjUBO uboData = {};
		uboData.projection = renderer.camera->projection();
		uboData.view = renderer.camera->view();

		DataBuffer data;
		data.data = &uboData;
		data.size = sizeof(InstancedQuadRenderer::ViewProjUBO);

		renderer.graphicsDevice->update_buffer(renderer.viewProjBuffer, data);
	}

	void instanced_quad_renderer_set_clear_color(InstancedQuadRenderer& renderer, const Color& color)
	{
		renderer.clearColor = color;
	}

	void instanced_quad_renderer_set_viewport(InstancedQuadRenderer& renderer, const Viewport& viewport)
	{
		renderer.viewport = viewport;
	}
}

namespace tur
{
	void instanced_quad_renderer_add_quad(InstancedQuadRenderer& renderer, const InstanceData& quadData)
	{
		InstanceData* data = (InstanceData*)renderer.instanceMappedData;
		data[renderer.quadAmount] = quadData;

		++renderer.quadAmount;
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