#pragma once
#include <turnip_engine.hpp>
#include "scene_data.hpp"

using namespace tur;

class SceneEditor
{
public:
	void initialize(NON_OWNING tur::GraphicsDevice* graphicsDevice, NON_OWNING Window* window, NON_OWNING SceneData* sceneData);

public:
	void on_render_gui();

private:
	NON_OWNING GraphicsDevice* r_GraphicsDevice = nullptr;
	NON_OWNING Window* r_Window = nullptr;
	NON_OWNING SceneData* r_SceneData = nullptr;
};