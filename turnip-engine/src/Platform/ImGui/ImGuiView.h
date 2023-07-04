#pragma once
#include "Core/View/View.h"

namespace tur
{
	class ImGuiView : public IView
	{
	public:
		void Begin();

		void End();

	public:
		void OnInitialize() override;

		void OnShutdown() override;
	};
}