#pragma once
#include <turnip_engine.hpp>
#include "scene_data.hpp"
#include "widget.hpp"

class SceneEditor : public Widget
{
public:
	void initialize(NON_OWNING GraphicsDevice* graphicsDevice, NON_OWNING GUISystem* guiSystem, NON_OWNING SceneData* sceneData);

public:
	void on_render_gui();

private:
	void setup_scene_texture();

private:
	NON_OWNING GraphicsDevice* r_GraphicsDevice = nullptr;
	NON_OWNING SceneData* r_SceneData = nullptr;
	NON_OWNING GUISystem* r_GUISystem = nullptr;

private:
	ImVec2 m_LatestSize;
	ImVec2 m_LatestPosition;
};