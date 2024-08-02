#pragma once
#include <imgui.h>

#include "Core/Config/ConfigData.hpp"
#include "Graphics/UI/UIBackend.hpp"

namespace tur
{
	class UISystem
	{
	public:
		void Initialize(const ConfigData& configData, platform::Window& window);

		void SetUIBackend(tur_unique<UIBackend> ui);

	public:
		void StartFrame();

		void EndFrame();

	private:
		tur_unique<UIBackend> m_UI = nullptr;
	};
}