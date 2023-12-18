#pragma once
#include "Common.h"
#include "Core/Event/Event.h"

namespace tur
{
	class ViewHolder;

	class View
	{
	public:
		explicit View(ViewHolder* handler = nullptr)
			: handler(handler)
		{

		}

		virtual ~View() = default;

	public:
		virtual void OnEngineInitialize() { };

		virtual void OnInstantiated() { };

		virtual void OnRender() { };

		virtual void OnRenderGUI() { };

		virtual void OnUpdate() { };

		virtual void OnEvent(Event& event) { };

		virtual void OnRemoved() { };

	public:
		void SetHandler(ViewHolder* handler) { this->handler = handler; }

	protected:
		NON_OWNING ViewHolder* handler = nullptr;
	};
}