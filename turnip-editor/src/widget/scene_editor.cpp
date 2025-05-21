#include "pch.hpp"
#include "scene_editor.hpp"
#include "event/scene_editor_events.hpp"

void SceneEditor::initialize(NON_OWNING tur::GraphicsDevice* graphicsDevice, NON_OWNING SceneData* sceneData)
{
	r_GraphicsDevice = graphicsDevice;
	r_SceneData = sceneData;

	initialize_scene_texture();
}

void SceneEditor::on_render_gui()
{
	ImGui::Begin("Scene Editor");
	ImVec2 dimensions = ImGui::GetContentRegionAvail();
	if (m_LatestSize.x != dimensions.x || m_LatestSize.y != dimensions.y)
	{
		m_LatestSize = ImGui::GetContentRegionAvail();
		resize_scene_texture();
		
		SceneEditorResize editorResize(m_LatestSize.x, m_LatestSize.y);
		callback(editorResize);
	}

	// TODO: disallow use of internal graphics data
	auto sceneTextureHandle = r_GraphicsDevice->get_textures().get(r_SceneData->sceneTexture).handle;
	ImGui::Image((void*)sceneTextureHandle, m_LatestSize);
	ImGui::End();
}

void SceneEditor::initialize_scene_texture()
{
	TextureDescriptor descriptor;
	descriptor.width = m_LatestSize.x > 0 ? static_cast<u32>(m_LatestSize.x) : 1;
	descriptor.height = m_LatestSize.y > 0 ? static_cast<u32>(m_LatestSize.y) : 1;

	r_SceneData->sceneTexture = r_GraphicsDevice->create_texture(descriptor);
	r_SceneData->mainCamera.set_orthogonal(0.0f, m_LatestSize.x, 0.f, m_LatestSize.y, -1.f, 1.f);
}

void SceneEditor::resize_scene_texture()
{
	r_GraphicsDevice->destroy_texture(r_SceneData->sceneTexture);
	initialize_scene_texture();
}