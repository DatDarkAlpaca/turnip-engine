#pragma once
#include <common.hpp>
#include "scene_data.hpp"

class SceneViewer
{
public:
	void initialize(NON_OWNING Scene* scene, SceneData* sceneData)
	{
		m_Scene = scene;
		m_SceneData = sceneData;
	}

public:
	void on_render_gui()
	{
		ImGui::Begin("Scene Viewer");

		auto& registry = m_Scene->get_registry();
		for (auto& [entity, sceneGraphComp] : registry.view<SceneGraphComponent>().each())
		{
			if (sceneGraphComp.parent == entt::null)
				render_scene_graph_node(entity);
		}

		ImGui::End();
	}

private:
	void render_scene_graph_node(entt::entity entity)
	{
		auto& registry = m_Scene->get_registry();

		if (!registry.valid(entity))
			return;

		SceneGraphComponent& entitySceneGraph = registry.get<SceneGraphComponent>(entity);
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

		if (m_SceneData->viewerSelectedEntity == entity)
			flags |= ImGuiTreeNodeFlags_Selected;

		NameComponent& entityName = registry.get<NameComponent>(entity);

		if (entitySceneGraph.children.empty())
		{
			flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			ImGui::TreeNodeEx((void*)(u64)entity, flags, "%s", entityName.name.c_str());

			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				m_SceneData->viewerSelectedEntity = entity;

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{

			}
			return;
		}

		if (ImGui::TreeNodeEx((void*)(u64)entity, flags, "%s", entityName.name.c_str()))
		{
			for (auto child : entitySceneGraph.children)
				render_scene_graph_node(child);

			ImGui::TreePop();
		}
	}

private:
	NON_OWNING Scene* m_Scene = nullptr;
	SceneData* m_SceneData = nullptr;

	entt::entity m_RenamingEntity;
	std::string m_RenameString;
};