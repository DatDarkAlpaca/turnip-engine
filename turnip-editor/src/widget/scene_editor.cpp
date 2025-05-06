#include "pch.hpp"
#include "scene_editor.hpp"

void SceneEditor::initialize(NON_OWNING tur::GraphicsDevice* graphicsDevice, NON_OWNING Window* window, NON_OWNING SceneData* sceneData)
{
	r_GraphicsDevice = graphicsDevice;
	r_SceneData = sceneData;
	r_Window = window;
}

void SceneEditor::on_render_gui()
{
	auto windowSize = r_Window->data.properties.dimensions;

	ImGui::Begin("Scene Editor");
	auto sceneTextureHandle = r_GraphicsDevice->get_textures().get(r_SceneData->sceneTexture).handle;
	ImGui::Image((void*)sceneTextureHandle, { (float)windowSize.x, (float)windowSize.y });
	ImGui::End();
}
