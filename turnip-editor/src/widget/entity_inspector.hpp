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
				ImGui::DragFloat3("##position", &transform.transform.position[0]);

				ImGui::Text("Rotation"); 
				ImGui::DragFloat3("##rotation", &transform.transform.rotation[0], 0.5f, 0.0f, 360.f);
			
				ImGui::Text("Scale"); 
				ImGui::DragFloat3("##scale", &transform.transform.scale[0]);

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