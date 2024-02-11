#pragma once
#include "Common.h"
#include "Core/Event/Event.h"

namespace tur
{
	class ViewSystem;

	class View
	{
	public:
		explicit View(ViewSystem* handler = nullptr)
			: handler(handler)
		{

		}

		virtual ~View() = default;

	public:
		virtual void OnEngineStartup() { };

		virtual void OnViewInstantiated() { };

		virtual void OnRender() { };

		virtual void OnRenderGUI() { };

		virtual void OnUpdate() { };

		virtual void OnEvent(Event& event) { };

		virtual void OnViewRemoved() { };

		virtual void OnEngineShutdown() { };

	public:
		void SetHandler(ViewSystem* handler) { this->handler = handler; }

	protected:
		NON_OWNING ViewSystem* handler = nullptr;
	};
}