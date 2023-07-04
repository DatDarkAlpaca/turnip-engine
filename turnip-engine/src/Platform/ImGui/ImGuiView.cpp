#include "pch.h"
#include "ImGuiView.h"
#include "Engine/TurnipEngine.h"

namespace tur
{
	void ImGuiView::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiView::End()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiView::OnInitialize()
	{
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		GLFWwindow* window = TurnipEngine::Get().GetWindow().Get();
		ImGui_ImplGlfw_InitForOpenGL(window, true);

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiView::OnShutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}

