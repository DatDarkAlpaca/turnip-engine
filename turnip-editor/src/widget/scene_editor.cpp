#include "pch.hpp"
#include "scene_editor.hpp"
#include "utils/gui_utils.hpp"
#include "event/scene_editor_events.hpp"

using namespace tur;

void SceneEditor::initialize(NON_OWNING tur::GraphicsDevice* graphicsDevice, NON_OWNING SceneData* sceneData)
{
	r_GraphicsDevice = graphicsDevice;
	r_SceneData = sceneData;

	initialize_scene_texture();
}
 
void SceneEditor::on_render_gui()
{
	ImGui::Begin("Scene Editor");
	
	ImGui::Texture(r_GraphicsDevice, r_SceneData->sceneTexture, m_LatestSize, { 0.0f, 1.0f }, { 1.0f, 0.0f });

	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		auto size = get_mouse_pixel_position(m_LatestSize.x, m_LatestSize.y);

		SceneEditorClicked editorClicked(size.x, size.y);
		callback(editorClicked);
	}

	if (r_SceneData->viewerSelectedEntity.is_valid())
	{
		auto& transform = r_SceneData->viewerSelectedEntity.get_component<TransformComponent>().transform;

		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetDrawlist(ImGui::GetCurrentWindow()->DrawList);

		ImGuizmo::SetRect(m_LatestPosition.x, m_LatestPosition.y, m_LatestSize.x, m_LatestSize.y);

		ImGuizmo::OPERATION operation = ImGuizmo::UNIVERSAL;
		ImGuizmo::MODE mode = ImGuizmo::LOCAL;

		bool manipulated = ImGuizmo::Manipulate(
			&r_SceneData->editorCamera.camera.view()[0][0],
			&r_SceneData->editorCamera.camera.projection()[0][0],
			operation,
			mode,
			&transform.raw_transform()[0][0]
		);

		if(manipulated)
			transform.decompose_transform();
	}
	
	ImVec2 dimensions = ImGui::GetWindowSize();
	if (m_LatestSize.x != dimensions.x || m_LatestSize.y != dimensions.y && dimensions.x > 0 && dimensions.y > 0)
	{
		m_LatestSize = dimensions;
		resize_scene_texture();
	}

	ImVec2 position = ImGui::GetItemRectMin();
	if (m_LatestPosition.x != position.x || m_LatestPosition.y != position.y)
	{
		m_LatestPosition = position;

		SceneEditorMoved editorMoved((u32)m_LatestPosition.x, (u32)m_LatestPosition.y);
		callback(editorMoved);
	}

	ImGui::End();
}

void SceneEditor::initialize_scene_texture()
{
	TextureDescriptor descriptor;
	descriptor.width = m_LatestSize.x <= 0 ? 1 : static_cast<u32>(m_LatestSize.x);
	descriptor.height = m_LatestSize.x <= 0 ? 1 : static_cast<u32>(m_LatestSize.y);
	
	r_SceneData->sceneTexture = r_GraphicsDevice->create_texture(descriptor);

	SceneEditorResized editorResize(descriptor.width, descriptor.height);
	callback(editorResize);
}

void SceneEditor::resize_scene_texture()
{
	r_GraphicsDevice->destroy_texture(r_SceneData->sceneTexture);
	initialize_scene_texture();
}