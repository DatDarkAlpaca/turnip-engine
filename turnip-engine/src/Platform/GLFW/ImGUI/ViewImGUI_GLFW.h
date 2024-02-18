#pragma once
#include <imgui_impl_glfw.h>	
#include <imgui_impl_opengl3.h>
#include <imgui_impl_vulkan.h>

#include "Core/View/View.h"
#include "Rendering/RenderingSystem.h"

namespace tur
{
	class ViewImGUI : public View
	{
	public:
		ViewImGUI(Window& window)
		{
			Initialize(window);
		}

	private:
		void Initialize(Window& window)
		{
			auto* glfwWindow = std::any_cast<GLFWwindow*>(window.GetHandle());

			IMGUI_CHECKVERSION();

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

			// TODO: Configure ImGUI settings
			ImGui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);

			// TODO: Implement in the rendering system
			ImGui_ImplOpenGL3_Init("#version 330");
		}
	};
}