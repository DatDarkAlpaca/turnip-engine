#pragma once
#include "Graphics/UI/UI.hpp"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace tur::gl
{
	class OGLUI : public UI
	{
	public:
		void StartFrame() override
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
		}

		void EndFrame() override
		{
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	};
}