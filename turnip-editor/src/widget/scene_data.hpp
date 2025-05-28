#pragma once
#include <entt/entt.hpp>
#include <turnip_engine.hpp>

#include "editor_camera.hpp"

using namespace tur;

struct SceneData
{
	Entity viewerSelectedEntity;
	texture_handle sceneTexture;
	EditorCamera editorCamera;

	bool projectEdited = false; // use events
};