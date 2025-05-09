#pragma once
#include <turnip_engine.hpp>
#include "scene_data.hpp"

using namespace tur;

class SceneViewer
{
public:
	void initialize(NON_OWNING Scene* scene, SceneData* sceneData);

public:
	void on_render_gui();

private:
	void render_scene_graph_node(entt::entity entity);

private:
	void add_empty_entity();

private:
	NON_OWNING Scene* m_Scene = nullptr;
	SceneData* m_SceneData = nullptr;

private:
	entt::entity m_RenamingEntity = entt::null;
	char m_RenameBuffer[32] = {};
};