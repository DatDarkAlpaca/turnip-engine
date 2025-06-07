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

public:
	ImVec2 get_size() const { return m_LatestSize; }
	ImVec2 get_pos() const { return m_LatestPosition; }

private:
	NON_OWNING GraphicsDevice* r_GraphicsDevice = nullptr;
	NON_OWNING SceneData* r_SceneData = nullptr;
	NON_OWNING GUISystem* r_GUISystem = nullptr;

private:
	ImVec2 m_LatestSize;
	ImVec2 m_LatestPosition;
};