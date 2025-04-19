#pragma once
#include "instanced_quad_renderer_data.hpp"

#include "graphics/graphics.hpp"
#include "core/config/config_data.hpp"

namespace tur
{
	class InstancedQuadRenderer
	{
	private:
		struct QuadData
		{
			glm::mat4 transform;
			u32 textureID = invalid_handle;
		};

		struct Vertex
		{
			glm::vec3 position;
			glm::vec2 uvs;
		};

		struct RendererDrawCommand
		{
			u32 count;
			u32 instanceCount;
			u32 firstIndex;
			u32 baseIndex;
			u32 baseInstance;
		};

		struct renderer_arguments
		{
			std::string vertexFilepath, fragmentFilepath;

			u32 maxInstanceCount = 0;
			u32 textureCount = 0;
			u32 textureWidth = 0, textureHeight = 0;
		};

	public:
		void initialize(const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera);

		void render();

	private:
		void initialize_pipeline();

	private:
		NON_OWNING GraphicsDevice* r_GraphicsDevice = nullptr;
		NON_OWNING Camera* r_Camera = nullptr;
		tur_unique<CommandBuffer> m_Commands;

		InstancedQuadRendererInformation m_RendererInfo;

	private:
		void* instanceBufferMappedData = nullptr;
		u32 quadAmount = 0;

		pipeline_handle pipeline = invalid_handle;

		buffer_handle vertexBuffer      = invalid_handle;
		buffer_handle indexBuffer       = invalid_handle;
		buffer_handle drawBuffer        = invalid_handle;
		buffer_handle instanceBuffer    = invalid_handle;
		buffer_handle viewProjectionUBO = invalid_handle;

		texture_handle textureArray = invalid_handle;
		u32 textureCount = 0;
	};
}