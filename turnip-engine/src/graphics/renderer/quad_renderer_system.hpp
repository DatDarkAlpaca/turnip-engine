#pragma once
#include "core/scene/common_components.hpp"
#include "core/scene/scene.hpp"
#include "quad_renderer.hpp"

namespace tur
{
	struct QuadRendererSystem
	{
	public:
		void initialize(Scene* scene, GraphicsDevice* device, Camera* camera)
		{
			m_Renderer.initialize(device, camera);
			r_Scene = scene;
		}

	public:
		void render()
		{
			auto& registry = r_Scene->get_registry();

			auto view = registry.view<TransformComponent>();
			for (auto [entity, transformComponent] : view.each())
			{
				const auto& transform = transformComponent.transform;

				QuadRenderer::QuadData quadData;
				quadData.transform = transform;

				m_Renderer.add_quad(quadData);
			}

			m_Renderer.render();
			m_Renderer.clear_quads();
		}

	public:
		QuadRenderer& get_renderer() { return m_Renderer; }

	private:
		QuadRenderer m_Renderer;
		NON_OWNING Scene* r_Scene;
	};
}