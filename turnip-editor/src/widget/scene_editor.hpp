#pragma once
#include <turnip_engine.hpp>
#include "scene_data.hpp"
#include "widget.hpp"

class SceneEditor : public Widget
{
public:
	void initialize(NON_OWNING tur::GraphicsDevice* graphicsDevice, NON_OWNING SceneData* sceneData);

public:
	void on_render_gui();

private:
	void initialize_scene_texture();
	void resize_scene_texture();

private:
	NON_OWNING GraphicsDevice* r_GraphicsDevice = nullptr;
	NON_OWNING SceneData* r_SceneData = nullptr;

private:
	ImVec2 m_LatestSize;
	ImVec2 m_LatestPosition;
};