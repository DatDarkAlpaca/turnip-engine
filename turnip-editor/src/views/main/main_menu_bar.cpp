#include "pch.hpp"
#include "main_view.hpp"
#include "main_menu_bar.hpp"
#include "event/events.hpp"

#include <core/scene/scene_serialization.hpp>

using namespace tur;

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
	auto projectFolderPath = std::filesystem::path(open_folder_dialog("Open Project Folder"));
	std::string projectName = projectFolderPath.filename().string();

	ProjectOptions options;
	{
		options.projectName = projectName;
		options.projectFolder = projectFolderPath;
		options.domainFilepath = r_MainView->engine->configData.scriptingInfo.mainDomainPath;
	}

	auto projectWrap = create_empty_project(options);
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
	SceneSerializer serializer(&r_MainView->scene, r_MainView->m_ProjectData.projectPath / "scene.json");
	serializer.serialize();

	callback(OnProjectSaved());
}
void MainMenuBar::on_file_save_as()
{
	// TODO: implement
	// callback(OnProjectSaved());
}
void MainMenuBar::on_file_close()
{
	turnip_engine_shutdown(*r_MainView->engine);
}
