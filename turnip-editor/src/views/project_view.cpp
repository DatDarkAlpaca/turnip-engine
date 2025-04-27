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
		static char projectNameBuffer[256] = "";

		ImGui::InputText("Project Name", projectNameBuffer, 256);
		
		if (ImGui::Button("Create New Project"))
		{
			projectName = projectNameBuffer;

			auto projectFolderPath = open_folder_dialog("Open Project Folder");

			auto projectWrap = create_empty_project(projectName, projectFolderPath);
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