#pragma once
#include <turnip_engine.hpp>
#include "scene_data.hpp"

using namespace tur;

class SceneEditor
{
public:
	void initialize(NON_OWNING tur::GraphicsDevice* graphicsDevice, NON_OWNING Window* window, NON_OWNING SceneData* sceneData)
	{
		r_GraphicsDevice = graphicsDevice;
		r_SceneData = sceneData;
		r_Window = window;
	}

public:
	void on_render_gui()
	{
		auto windowSize = r_Window->data.properties.dimensions;

		ImGui::Begin("Scene Editor");
			auto sceneTextureHandle = r_GraphicsDevice->get_textures().get(r_SceneData->sceneTexture).handle;
			ImGui::Image((void*)sceneTextureHandle, { (float)windowSize.x, (float)windowSize.y });
		ImGui::End();
	}

private:
	NON_OWNING GraphicsDevice* r_GraphicsDevice = nullptr;
	NON_OWNING Window* r_Window = nullptr;
	NON_OWNING SceneData* r_SceneData = nullptr;
};