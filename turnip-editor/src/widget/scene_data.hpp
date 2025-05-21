#pragma once
#include <entt/entt.hpp>
#include <turnip_engine.hpp>

using namespace tur;

struct SceneData
{
	Entity viewerSelectedEntity;
	texture_handle sceneTexture;
	Camera mainCamera;

	bool projectEdited = false; // use events
};