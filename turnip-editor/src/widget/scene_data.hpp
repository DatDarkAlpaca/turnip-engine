#pragma once
#include <entt/entt.hpp>
#include <turnip_engine.hpp>

#include "editor_camera.hpp"

using namespace tur;

struct SceneData
{
	texture_handle sceneTexture = invalid_handle;
	render_target_handle sceneRenderTarget = invalid_handle;

	Entity viewerSelectedEntity;
	EditorCamera editorCamera;
};