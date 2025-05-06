#include "pch.hpp"
#include "scene_viewer.hpp"

void SceneViewer::initialize(NON_OWNING Scene* scene, SceneData* sceneData)
{
	m_Scene = scene;
	m_SceneData = sceneData;
}

void SceneViewer::on_render_gui()
{
	ImGui::Begin("Scene Viewer");

	auto& registry = m_Scene->get_registry();
	for (auto& [entity, sceneGraphComp] : registry.view<SceneGraphComponent>().each())
	{
		if (sceneGraphComp.parent == entt::null)
			render_scene_graph_node(entity);
	}

	ImGui::Dummy(ImVec2(0, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeight()));
	if (ImGui::Button("Add Entity", ImVec2(-FLT_MIN, 0)))
		add_empty_entity();

	ImGui::End();
}

void SceneViewer::render_scene_graph_node(entt::entity entity)
{
	auto& registry = m_Scene->get_registry();

	if (!registry.valid(entity))
		return;

	SceneGraphComponent& entitySceneGraph = registry.get<SceneGraphComponent>(entity);
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if (m_SceneData->viewerSelectedEntity == entity)
		flags |= ImGuiTreeNodeFlags_Selected;

	NameComponent& entityName = registry.get<NameComponent>(entity);

	if (!entitySceneGraph.children.empty())
	{
		if (ImGui::TreeNodeEx((void*)(u64)entity, flags, "%s", entityName.name.c_str()))
		{
			for (auto child : entitySceneGraph.children)
				render_scene_graph_node(child);

			ImGui::TreePop();
		}
	}

	if (m_RenamingEntity != entt::null)
	{
		ImGui::SetNextItemWidth(150.0f);

		std::copy(entityName.name.begin(), entityName.name.end(), m_RenameBuffer);

		if (ImGui::InputText("##Editing", m_RenameBuffer, IM_ARRAYSIZE(m_RenameBuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll))
		{
			entityName.name = std::string(m_RenameBuffer);
			m_RenamingEntity = entt::null;
		}
	}

	else
	{
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		ImGui::TreeNodeEx((void*)(u64)entity, flags, "%s", entityName.name.c_str());

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("SceneViewerPopup");
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			m_SceneData->viewerSelectedEntity = entity;

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			m_RenamingEntity = entity;

		if (ImGui::BeginPopup("SceneViewerPopup"))
			render_scene_viewer_popup();
	}

	// TODO: move entities
	// TODO: entity hierarchy
	// TODO: use projectEdited
}

void SceneViewer::render_scene_viewer_popup()
{
	if (ImGui::Button("Export as instance"))
	{
		// 1. get all components from entity
		// 2. save to file
	}

	ImGui::EndPopup();
}

void SceneViewer::add_empty_entity()
{
	m_SceneData->viewerSelectedEntity = m_Scene->add_entity();
	m_SceneData->projectEdited = true;
}