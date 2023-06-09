#pragma once
#include "Event/IEvent.h"

namespace tur
{
	class IView
	{
	public:
		IView() = default;
		virtual ~IView() = default;

	public:
		virtual void OnInitialize() { };

		virtual void OnShutdown() { };

		virtual void OnRenderGUI() { };

		virtual void OnUpdate() { };

		virtual void OnEvent(IEvent& event) { };
	};
}