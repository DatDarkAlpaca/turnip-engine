#pragma once
#include <imgui.h>

#include "Core/Config/ConfigSystem.hpp"
#include "Graphics/UI/UIBackend.hpp"

namespace tur
{
	class UISystem
	{
	public:
		void Initialize(const ConfigSystem& configSystem, platform::Window& window);

		void SetUIBackend(tur_unique<UIBackend> ui);

	public:
		void StartFrame();

		void EndFrame();

	private:
		tur_unique<UIBackend> m_UI = nullptr;
	};
}