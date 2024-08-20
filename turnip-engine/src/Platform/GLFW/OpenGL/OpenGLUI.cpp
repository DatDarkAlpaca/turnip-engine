#include "pch.hpp"
//#include "OpenGLUI.hpp"
//
//namespace tur
//{
//	void OpenGLUI::Initialize(platform::Window& window)
//	{
//		ImGui_ImplGlfw_InitForOpenGL(window.GetHandle(), true);
//		ImGui_ImplOpenGL3_Init(g_OpenGLVersion);
//	}
//
//	void OpenGLUI::StartFrame()
//	{
//		ImGui_ImplOpenGL3_NewFrame();
//		ImGui_ImplGlfw_NewFrame();
//		ImGui::NewFrame();
//	}
//
//	void OpenGLUI::EndFrame()
//	{
//		ImGui::Render();
//		glClear(GL_COLOR_BUFFER_BIT);
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//		ImGuiIO& io = ImGui::GetIO();
//		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//		{
//			GLFWwindow* backup_current_context = glfwGetCurrentContext();
//			ImGui::UpdatePlatformWindows();
//			ImGui::RenderPlatformWindowsDefault();
//
//			glfwMakeContextCurrent(backup_current_context);
//		}
//	}
//}
//
