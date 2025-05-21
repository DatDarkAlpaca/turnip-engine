#pragma once
#include <common.hpp>
#include "core/scene/scene.hpp"
#include "scene_data.hpp"
#include "widget.hpp"

class EntityInspector : public Widget
{
public:
	void initialize(NON_OWNING tur::TurnipEngine* engine, NON_OWNING tur::Scene* scene, SceneData* sceneData);

public:
	void on_render_gui();

	void on_event(Event& event) override;

private:
	void render_components(Entity selectedEntity);
	void render_transform_component(Entity selectedEntity);
	void render_texture_component(Entity selectedEntity);
	void render_script_component(Entity selectedEntity);

private:
	void render_component_add_list(Entity selectedEntity);

private:
	void render_add_transform_component(Entity selectedEntity);
	void render_add_texture_component(Entity selectedEntity);
	void render_add_script_component(Entity selectedEntity);

private:
	NON_OWNING tur::TurnipEngine* r_Engine = nullptr;
	NON_OWNING Scene* r_Scene = nullptr;
	SceneData* r_SceneData = nullptr;

private:
	Rect2D m_EditorArea;
};