#pragma once
#include "core/scene/common_components.hpp"
#include "core/scene/scene.hpp"
#include "instanced_quad_renderer.hpp"

namespace tur
{
	struct InstancedQuadRendererSystem
	{
	public:
		void initialize(const ConfigData& configData, GraphicsDevice* device, Camera* camera = nullptr, Scene* scene = nullptr)
		{
			m_Renderer.initialize(configData, device, camera);
			r_Scene = scene;
		}

		void set_camera(Camera* camera)
		{
			m_Renderer.set_camera(camera);
		}
		void set_scene(Scene* scene)
		{
			r_Scene = scene;
		}

	public:
		void render()
		{
			TUR_LOG_CRITICAL("Instanced Quad Renderering is not available as an engine system yet");

			auto& registry = r_Scene->get_registry();

			auto view0 = registry.view<TransformComponent>(entt::exclude<TextureComponent>);
			for (auto [entity, transformComponent] : view0.each())
			{
				// TODO: implement
			}

			auto view1 = registry.view<TransformComponent, TextureComponent>();
			for (auto [entity, transformComponent, textureComponent] : view1.each())
			{
				// TODO: implement
			}

			m_Renderer.render();		
		}

	public:
		InstancedQuadRenderer& get_renderer() { return m_Renderer; }
		InstancedQuadRenderer& get() { return m_Renderer; }

	private:
		InstancedQuadRenderer m_Renderer;
		NON_OWNING Scene* r_Scene = nullptr;
	};
}