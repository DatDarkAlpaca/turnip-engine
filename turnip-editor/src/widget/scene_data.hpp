#pragma once
#include <entt/entt.hpp>

struct SceneData
{
	entt::entity viewerSelectedEntity = entt::null;
	texture_handle sceneTexture;
};