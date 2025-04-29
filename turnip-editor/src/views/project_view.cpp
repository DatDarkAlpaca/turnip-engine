#include "pch.hpp"
#include "project_view.hpp"
#include "views/main/main_view.hpp"

void ProjectView::on_engine_startup()
{
	r_Engine->get_view_system().views.reserve(2);
	show_window(&r_Engine->get_window());
}

void ProjectView::on_update()
{
	if (!m_IsOpen)
		r_Engine->shutdown();
}
void ProjectView::on_render_gui()
{
	ImGui::SetNextWindowViewport(0);

	if (ImGui::Begin("Project Editor", &m_IsOpen, ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::Button("Create New Project"))
		{
			auto projectFolderPath = std::filesystem::path(open_folder_dialog("Open Project Folder"));
			std::string projectName = projectFolderPath.filename().string();

			ProjectOptions options;
			{
				options.projectName = projectName;
				options.projectFolder = projectFolderPath;
				options.domainFilepath = r_Engine->get_config_data().scriptingInfo.mainDomainPath;
			}

			auto projectWrap = create_empty_project(options);
			if (!projectWrap.has_value())
				return ImGui::End();

			r_Engine->add_view(tur::make_unique<MainView>(projectWrap.value()));
			r_Engine->remove_view(viewHandle);
		}

		if (ImGui::Button("Open Project..."))
		{
			auto projectFilepaths = open_file_dialog("Open Project Folder", { "Project Files (.json)" });

			if(projectFilepaths.empty())
				return ImGui::End();

			auto projectWrap = read_project_file(projectFilepaths[0]);
			if (!projectWrap.has_value())
				return ImGui::End();

			r_Engine->add_view(tur::make_unique<MainView>(projectWrap.value()));
			r_Engine->remove_view(viewHandle);
		}

		ImGui::End();
	}
}