#pragma once
#include <imgui.h>
#include <turnip_engine.hpp>

#include "main_menu_bar.hpp"

#include "widget/scene_data.hpp"
#include "widget/scene_viewer.hpp"
#include "widget/entity_inspector.hpp"
#include "widget/scene_editor.hpp"

using namespace tur;

class MainView : public tur::View
{
	friend class MainMenuBar;

public:
	explicit MainView(const ProjectData& projectData);

public:
	void set_project_data(const ProjectData& projectData);
	
public:
	void on_view_added() override;
	void on_update() override;
	void on_render_gui() override;
	void on_event(Event& event) override;
	void on_render() override;

private:
	void initialize_textures();
	void initialize_renderer_system();

private:
	void append_window_title(const std::string& extraText);

	void update_render_target(u32 width, u32 height);

private:
	SceneData m_SceneData;
	MainMenuBar m_MainMenuBar;

	SceneViewer m_SceneViewer;
	SceneEditor m_SceneEditor;
	EntityInspector m_EntityInspector;

private:
	NON_OWNING QuadRenderer* r_QuadRenderer = nullptr;
	NON_OWNING InstancedQuadRenderer* r_InstancedRenderer = nullptr;

private:
	ProjectData m_ProjectData;
};