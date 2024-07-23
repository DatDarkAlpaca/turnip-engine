#include "pch.hpp"
#include "UISystem.hpp"

namespace tur
{
	void UISystem::Initialize(const ConfigSystem& configSystem, platform::Window& window)
	{
		// TODO: use the config system to setup the ui options

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// TODO: enable options
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Platform:
		m_UI->Initialize(window);
	}

	void UISystem::SetUIBackend(tur_unique<UIBackend> ui)
	{
		m_UI = std::move(ui);
	}

	void UISystem::StartFrame()
	{
		m_UI->StartFrame();
	}

	void UISystem::EndFrame()
	{
		m_UI->EndFrame();
	}	
}

