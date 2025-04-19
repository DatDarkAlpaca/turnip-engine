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
		// initialize_buffers();
    }

	void InstancedQuadRenderer::initialize_pipeline()
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
				attribute0.offset = offsetof(InstancedQuadRenderer::Vertex, position);
				vertexInput.attributes.push_back(attribute0);
			}

			Attribute attribute1;
			{
				attribute1.binding = 0;
				attribute1.location = 1;
				attribute1.format = AttributeFormat::R32G32_SFLOAT;
				attribute0.offset = offsetof(InstancedQuadRenderer::Vertex, uvs);
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
}