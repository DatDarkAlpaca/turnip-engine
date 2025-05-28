#pragma once
#include "instanced_quad_renderer_data.hpp"

#include "graphics/graphics.hpp"
#include "core/config/config_data.hpp"

#include "utils/color.hpp"

namespace tur
{
	struct InstancedQuadRenderer
	{
	public:
		struct Data
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

		struct ViewProjUBO
		{
			glm::mat4 view;
			glm::mat4 projection;
		};

	public:
		NON_OWNING GraphicsDevice* graphicsDevice = nullptr;
		NON_OWNING Camera* camera = nullptr;
		tur_unique<CommandBuffer> commands;

		InstancedQuadRendererInformation info;

		Color clearColor;
		Viewport viewport;

	public:
		pipeline_handle pipeline			= invalid_handle;
		buffer_handle vertexBuffer			= invalid_handle;
		buffer_handle indexBuffer			= invalid_handle;
		buffer_handle viewProjBuffer     	= invalid_handle;
		buffer_handle instanceBuffer		= invalid_handle;
		texture_handle textureArray			= invalid_handle;

	public:
		void* instanceMappedData = nullptr;
		u32 quadAmount = 0;
		u32 textureCount = 0;
	};
	using InstanceData = InstancedQuadRenderer::Data;

	void initialize_instanced_quad_renderer(InstancedQuadRenderer& renderer, const ConfigData& configData, GraphicsDevice* graphicsDevice, Camera* camera);
	void instanced_quad_renderer_render(InstancedQuadRenderer& renderer);

	void instanced_quad_renderer_set_camera(InstancedQuadRenderer& renderer, Camera* camera);
	void instanced_quad_renderer_set_clear_color(InstancedQuadRenderer& renderer, const Color& color);
	void instanced_quad_renderer_set_viewport(InstancedQuadRenderer& renderer, const Viewport& viewport);

	void instanced_quad_renderer_add_quad(InstancedQuadRenderer& renderer, const InstanceData& quadData);
	void instanced_quad_renderer_add_texture(InstancedQuadRenderer& renderer, const TextureAsset& asset);
}