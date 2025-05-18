#pragma once
#include "instanced_quad_renderer_data.hpp"

#include "graphics/graphics.hpp"
#include "core/config/config_data.hpp"

namespace tur
{
	class InstancedQuadRenderer
	{
	private:
		struct InstanceData
		{
			glm::vec3 position;
			glm::vec2 scale;
			u32 textureLayer;
		};

		struct Vertex
		{
			glm::vec3 position;
			glm::vec2 uvs;
		};
		
		struct VPUBO
		{
			glm::mat4 view;
			glm::mat4 projection;
		};
	
	public:
		void initialize(const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera);
		void render();

	public:
		void set_camera(Camera* camera);

		void set_clear_color(const glm::vec4& color);
		void set_viewport(const Viewport& viewport);

		void add_quad(const InstanceData& quadData);
		void add_texture(const TextureAsset& asset);

	private:
		void initialize_pipeline();
		void initialize_buffers();
		void initialize_textures();

	private:
		NON_OWNING GraphicsDevice* r_GraphicsDevice = nullptr;
		NON_OWNING Camera* r_Camera = nullptr;
		tur_unique<CommandBuffer> m_Commands;

		InstancedQuadRendererInformation m_RendererInfo;

		glm::vec4 m_ClearColor;
		Viewport m_Viewport;

	private:
		pipeline_handle pipeline = invalid_handle;

		buffer_handle vertexBuffer      = invalid_handle;
		buffer_handle indexBuffer       = invalid_handle;
		buffer_handle vpBuffer          = invalid_handle;
		buffer_handle instanceBuffer    = invalid_handle;

		texture_handle textureArray = invalid_handle;

		void* instanceMappedData = nullptr;
		u32 quadAmount = 0;
		u32 textureCount = 0;
	};
}