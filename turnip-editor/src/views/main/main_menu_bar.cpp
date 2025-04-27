#include "pch.hpp"
#include "main_view.hpp"
#include "main_menu_bar.hpp"

#include <core/scene/scene_serialization.hpp>

void MainMenuBar::initialize(NON_OWNING MainView* mainView)
{
	r_MainView = mainView;
}

void MainMenuBar::on_render()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N"))
				on_file_new();

			if (ImGui::MenuItem("Open...", "Ctrl+O"))
				on_file_open();

			if (ImGui::MenuItem("Save", "Ctrl+S"))
				on_file_save();

			if (ImGui::MenuItem("Save As...", "Ctrl+Alt+S"))
				on_file_save_as();

			ImGui::Separator();

			if (ImGui::MenuItem("Close", "Ctrl+W"))
				on_file_close();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void MainMenuBar::on_file_new()
{
	// TODO: open popup

	static char projectNameBuffer[256] = "";
	static std::string projectName = projectNameBuffer;

	auto projectFolderPath = open_folder_dialog("Open Project Folder");

	auto projectWrap = create_empty_project(projectName, projectFolderPath);
	if (!projectWrap.has_value())
		return;

	r_MainView->set_project_data(projectWrap.value());
}
void MainMenuBar::on_file_open()
{
	auto projectFilepaths = open_file_dialog("Open Project Folder", { "Project Files (.json)" });

	if (projectFilepaths.empty())
		return;

	auto projectWrap = read_project_file(projectFilepaths[0]);
	if (!projectWrap.has_value())
		return;

	r_MainView->set_project_data(projectWrap.value());
}
void MainMenuBar::on_file_save()
{
	SceneSerializer serializer(&r_MainView->m_Scene, r_MainView->m_ProjectData.projectPath / "scene.json");
	serializer.serialize();
}
void MainMenuBar::on_file_save_as()
{
	// TODO: scene serialization
}
void MainMenuBar::on_file_close()
{
	r_MainView->r_Engine->shutdown();
}
