#pragma once
#include <imgui_impl_glfw.h>	
#include <imgui_impl_opengl3.h>

#include "Core/View/View.h"
#include "Rendering/RenderingSystem.h"
#include "Rendering/GUI/ImGuiView.h"

namespace tur
{
	class ImGUIGLView : public ImGuiView
	{
	public:
		ImGUIGLView(Window& window, const GUIOptions& options)
		{
			Initialize(window, options);
		}

	private:
		void Initialize(Window& window, const GUIOptions& options)
		{
			auto* glfwWindow = std::any_cast<GLFWwindow*>(window.GetHandle());

			IMGUI_CHECKVERSION();

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

			// TODO: Configure ImGUI settings using GUIOptions
			ImGui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);

			// TODO: Implement in the rendering system
			ImGui_ImplOpenGL3_Init("#version 330");
		}
	};
}