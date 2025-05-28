#pragma once
#include <turnip_engine.hpp>
#include "scene_data.hpp"
#include "widget.hpp"

class SceneViewer : public Widget
{
public:
	void initialize(NON_OWNING Scene* scene, SceneData* sceneData);

public:
	void on_render_gui();

private:
	void render_scene_graph_node(Entity entity);

	void render_scene_viewer_popup(Entity entity);

private:
	void add_empty_entity();

private:
	NON_OWNING Scene* r_Scene = nullptr;
	NON_OWNING SceneData* r_SceneData = nullptr;

private:
	entt::entity m_RenamingEntity = entt::null;
	char m_RenameBuffer[32] = {};
};