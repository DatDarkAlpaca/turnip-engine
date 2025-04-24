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

		if (m_SceneData->viewerSelectedEntity == entt::null)
		{
			ImGui::End();
			return;
		}
		
		Entity entity(m_SceneData->viewerSelectedEntity, m_Scene);

		render_components(entity);

		ImGui::Separator();
		
		render_component_list(entity);

		ImGui::End();
	}

private:
	void render_components(Entity selectedEntity)
	{
		if (selectedEntity.has_component<TransformComponent>())
			render_transform_component(selectedEntity);
	}

	void render_transform_component(Entity selectedEntity)
	{
		auto& transform = selectedEntity.get_component<TransformComponent>().transform;

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Position");
			ImGui::DragFloat3("##position", &transform.position[0]);

			ImGui::Text("Rotation");
			ImGui::DragFloat3("##rotation", &transform.rotation[0], 0.5f, 0.0f, 360.f);

			ImGui::Text("Scale");
			ImGui::DragFloat3("##scale", &transform.scale[0]);
		}		
	}

private:
	void render_component_list(Entity selectedEntity)
	{
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponentPopup");

		if (ImGui::BeginPopup("AddComponentPopup")) 
		{
			if (!selectedEntity.has_component<TransformComponent>() && ImGui::MenuItem("Transform"))
			{
				Transform transform(glm::mat4(1.0f));
				selectedEntity.add_component<Transform>(transform);
			}
			
			ImGui::EndPopup();
		}
	}

private:
	NON_OWNING Scene* m_Scene = nullptr;
	SceneData* m_SceneData = nullptr;
};