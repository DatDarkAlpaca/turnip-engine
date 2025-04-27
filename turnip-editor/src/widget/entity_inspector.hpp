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
			return ImGui::End();
		
		Entity entity(m_SceneData->viewerSelectedEntity, m_Scene);

		render_components(entity);

		ImGui::Separator();
		
		render_component_add_list(entity);

		ImGui::End();
	}

private:
	void render_components(Entity selectedEntity)
	{
		if (selectedEntity.has_component<TransformComponent>())
			render_transform_component(selectedEntity);
		
		if(!selectedEntity.get_component<EntityScriptsComponent>().scriptComponents.empty())
			render_script_component(selectedEntity);
	}

	void render_transform_component(Entity selectedEntity)
	{
		auto& transform = selectedEntity.get_component<TransformComponent>().transform;

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Position");
			ImGui::SameLine();
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
			ImGui::DragFloat3("##position", &transform.position[0]);
			ImGui::PopItemWidth();

			ImGui::Text("Rotation");
			ImGui::SameLine();
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
			ImGui::DragFloat3("##rotation", &transform.rotation[0]);
			ImGui::PopItemWidth();

			ImGui::Text("Scale");
			ImGui::SameLine();
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
			ImGui::DragFloat3("##scale", &transform.scale[0]);
			ImGui::PopItemWidth();
		}		
	}

	void render_script_component(Entity selectedEntity)
	{
		auto& scripts = selectedEntity.get_component<EntityScriptsComponent>().scriptComponents;

		for (const auto& script : scripts)
		{
			if (ImGui::CollapsingHeader(script.className.c_str()))
				ImGui::Text(script.className.c_str());
		}
	}

private:
	void render_component_add_list(Entity selectedEntity)
	{
		ImGui::Dummy(ImVec2(0, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeight()));
		if (ImGui::Button("Add Component", ImVec2(-FLT_MIN, 0)))
			ImGui::OpenPopup("AddComponentPopup");

		if (ImGui::BeginPopup("AddComponentPopup")) 
		{
			if (!selectedEntity.has_component<TransformComponent>() && ImGui::MenuItem("Transform"))
				selectedEntity.add_component<TransformComponent>();

			if (ImGui::MenuItem("Entity Script"))
			{
				// TODO: create a new .cs file. Ask which directory etc
				InternalEntityScript script("Test");
				selectedEntity.get_component<EntityScriptsComponent>().add(script);
			}
			
			ImGui::EndPopup();
		}
	}

private:
	NON_OWNING Scene* m_Scene = nullptr;
	SceneData* m_SceneData = nullptr;
};