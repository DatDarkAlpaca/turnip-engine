#pragma once
#include <common.hpp>
#include "core/scene/scene.hpp"
#include "scene_data.hpp"

class EntityInspector
{
public:
	void initialize(NON_OWNING tur::Scene* scene, SceneData* sceneData)
	{
		m_Scene = scene;
		m_SceneData = sceneData;
	}

public:
	void on_render_gui()
	{
		ImGui::Begin("Entity Inspector");

		Entity entity(m_SceneData->viewerSelectedEntity, m_Scene);

		if (entity.has_component<TransformComponent>())
		{
			TransformComponent& transform = entity.get_component<TransformComponent>();

			ImGui::BeginGroup();

			ImGui::Text("Position"); 
			ImGui::DragFloat3("##position", &transform.position[0]);

			ImGui::EndGroup();
		}

		ImGui::End();
	}

private:
	NON_OWNING Scene* m_Scene = nullptr;
	SceneData* m_SceneData = nullptr;

	entt::entity m_SelectedEntity;
	entt::entity m_RenamingEntity;
	std::string m_RenameString;
};