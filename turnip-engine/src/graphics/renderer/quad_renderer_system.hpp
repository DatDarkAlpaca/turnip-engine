#pragma once
#include "core/scene/common_components.hpp"
#include "core/scene/scene.hpp"
#include "quad_renderer.hpp"

namespace tur
{
	struct QuadRendererSystem
	{
	public:
		void initialize(const ConfigData& configData, GraphicsDevice* device, Camera* camera, Scene* scene)
		{
			m_Renderer.initialize(configData, device, camera);
			r_Scene = scene;
		}

	public:
		void render()
		{
			auto& registry = r_Scene->get_registry();

			auto view0 = registry.view<TransformComponent>(entt::exclude<TextureComponent>);
			for (auto [entity, transformComponent] : view0.each())
			{
				QuadRenderer::QuadData quadData = {};
				quadData.transform = transformComponent.transform.transform();
				quadData.texture = invalid_handle;

				m_Renderer.add_quad(quadData);
			}

			auto view1 = registry.view<TransformComponent, TextureComponent>();
			for (auto [entity, transformComponent, textureComponent] : view1.each())
			{
				const auto& texture = textureComponent.handle;
				
				QuadRenderer::QuadData quadData = {};
				quadData.transform = transformComponent.transform.transform();
				quadData.texture = texture;

				m_Renderer.add_quad(quadData);
			}

			m_Renderer.render();
			m_Renderer.clear_quads();
		}

	public:
		QuadRenderer& get_renderer() { return m_Renderer; }

	private:
		QuadRenderer m_Renderer;
		NON_OWNING Scene* r_Scene = nullptr;
	};
}