#include "pch.hpp"
#include "quad_renderer.hpp"

namespace tur::quad_renderer
{
	static void initialize_pipeline(QuadRenderer& renderer, const QuadRendererInformation& information)
	{
		// Vertex Input:
		VertexInputDescriptor vertexInput;
		{
			BindingDescription bindingDescription;
			{
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(QuadRenderer::Vertex);
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

		// Shaders:
		shader_handle vertexShader = renderer.graphicsDevice->create_shader(ShaderDescriptor{
			information.vertexFilepath,
			ShaderType::VERTEX
		});
		shader_handle fragmentShader = renderer.graphicsDevice->create_shader(ShaderDescriptor{
			information.fragmentFilepath,
			ShaderType::FRAGMENT
		});

		// Descriptor Layout:
		DescriptorSetLayoutDescriptor descriptorDescriptor;
		{
			DescriptorSetLayoutEntry entry0;
			{
				entry0.binding = 0;
				entry0.amount = 1;
				entry0.type = DescriptorType::UNIFORM_BUFFER;
				entry0.stage = PipelineStage::VERTEX_STAGE;
			}
			descriptorDescriptor.push_back(entry0);

			DescriptorSetLayoutEntry entry1;
			{
				entry1.binding = 1;
				entry1.amount = 1;
				entry1.type = DescriptorType::COMBINED_IMAGE_SAMPLER;
				entry1.stage = PipelineStage::FRAGMENT_STAGE;
			}
			descriptorDescriptor.push_back(entry1);
		}

		renderer.descriptor = renderer.graphicsDevice->create_descriptors(descriptorDescriptor);
		renderer.defaultDescriptorSet = renderer.graphicsDevice->create_descriptor_set(renderer.descriptor);
		
		// Pipeline:
		PipelineDescriptor descriptor;
		descriptor.vertexInputStage = vertexInput;
		descriptor.fragmentShader = fragmentShader;
		descriptor.vertexShader = vertexShader;
		descriptor.rasterizerStage = rasterizer;
		descriptor.descriptorSetLayouts.push_back(renderer.descriptor);

		renderer.pipeline = renderer.graphicsDevice->create_graphics_pipeline(descriptor);
	}

	static void initialize_buffers(QuadRenderer& renderer)
	{
		auto& graphicsDevice = renderer.graphicsDevice;

		// Vertex Buffer:
		{
			BufferDescriptor bufferDesc = {};
			{
				bufferDesc.type = BufferType::VERTEX_BUFFER;
			}

			DataBuffer data;
			QuadRenderer::Vertex vertices[4] = {
				{{ -0.5f, -0.5f, 0.0f },	{ 0.0f, 0.0f }},
				{{  0.5f, -0.5f, 0.0f },	{ 1.0f, 0.0f }},
				{{  0.5f,  0.5f, 0.0f },	{ 1.0f, 1.0f }},
				{{ -0.5f,  0.5f, 0.0f },	{ 0.0f, 1.0f }},
			};
			data.data = vertices;
			data.size = sizeof(vertices);

			renderer.buffer = graphicsDevice->create_default_buffer(bufferDesc, data);
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

			renderer.indexBuffer = graphicsDevice->create_default_buffer(bufferDesc, data);
		}

		// Uniform Buffer:
		{
			BufferDescriptor bufferDesc = {};
			{
				bufferDesc.type = BufferType::UNIFORM_BUFFER | BufferType::TRANSFER_DST;
				bufferDesc.usage = BufferUsage::DYNAMIC;
			}

			renderer.uniformBuffer = renderer.graphicsDevice->create_buffer(bufferDesc, sizeof(QuadRenderer::UBO));
		}
		
		renderer.graphicsDevice->update_descriptor_resource(renderer.defaultDescriptorSet, renderer.uniformBuffer, DescriptorType::UNIFORM_BUFFER, 0);	
	}
	
	static void bind_mvp(QuadRenderer& renderer, const glm::mat4& transform)
	{
		QuadRenderer::UBO ubo { transform, renderer.camera->view(), renderer.camera->projection() };

		DataBuffer dataBuffer;
		dataBuffer.size = sizeof(ubo);
		dataBuffer.data = &ubo;

		renderer.graphicsDevice->update_buffer(renderer.uniformBuffer, dataBuffer);
	}
}

namespace tur
{
	void initialize_quad_renderer(QuadRenderer& renderer, const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera)
	{
		renderer.graphicsDevice = graphicsDevice;
		renderer.camera = camera;

		renderer.commands = tur::make_unique<CommandBuffer>(graphicsDevice->create_command_buffer());
		renderer.commands->initialize();

		quad_renderer::initialize_pipeline(renderer, configData.quadRendererInformation);
		quad_renderer::initialize_buffers(renderer);
	}
	
	void quad_renderer_begin(QuadRenderer& renderer, render_target_handle renderTarget)
	{
		auto& commands = renderer.commands;
		commands->begin_render(renderTarget);
	}

	void quad_renderer_render(QuadRenderer& renderer)
	{
		auto& commands = renderer.commands;

		commands->set_viewport(renderer.viewport);
		commands->set_scissor(Rect2D{ 0, 0, renderer.viewport.width, renderer.viewport.height });
		
		if (renderer.shouldClear)
			commands->clear(ClearFlags::COLOR, ClearValue{ renderer.clearColor });

		commands->bind_vertex_buffer(renderer.buffer, 0, sizeof(QuadRenderer::Vertex));
		commands->bind_index_buffer(renderer.indexBuffer);
		commands->bind_pipeline(renderer.pipeline);	

		for (const auto& quad : renderer.quads)
		{
			quad_renderer::bind_mvp(renderer, quad.transform);

			if (quad.texture != invalid_handle)
				commands->bind_descriptor_set(quad.descriptorSet);

			else
			{
				if (renderer.defaultTexture != invalid_handle)
					commands->bind_descriptor_set(renderer.defaultDescriptorSet);
			}

			commands->draw_indexed(6);
		}
	}

	void quad_renderer_end(QuadRenderer& renderer)
	{
		auto& commands = renderer.commands;
		commands->end_render();
	}
}

namespace tur
{
	void quad_renderer_set_default_texture(QuadRenderer& renderer, texture_handle handle)
	{
		renderer.defaultTexture = handle;
		renderer.graphicsDevice->update_descriptor_resource(renderer.defaultDescriptorSet, renderer.defaultTexture, DescriptorType::COMBINED_IMAGE_SAMPLER, 1);
	}
}

namespace tur
{
	void quad_renderer_add_quad(QuadRenderer& renderer, QuadRenderer::Data& quad)
	{
		renderer.graphicsDevice->update_descriptor_resource(quad.descriptorSet, renderer.uniformBuffer, DescriptorType::UNIFORM_BUFFER, 0);

		if (quad.texture != invalid_handle)
			renderer.graphicsDevice->update_descriptor_resource(quad.descriptorSet, quad.texture, DescriptorType::COMBINED_IMAGE_SAMPLER, 1);

		renderer.quads.push_back(quad);
	}

	void quad_renderer_clear_quads(QuadRenderer& renderer)
	{
		renderer.quads.clear();
	}
}