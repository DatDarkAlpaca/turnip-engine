#pragma once
#include <turnip_engine.hpp>
#include "editor_events.hpp"

namespace tur
{
	class OnEntityInspectorInitialize : public Event
	{
		DEFINE_EVENT(EditorEvents::ON_EDITOR_INIT);

	public:
		OnEntityInspectorInitialize() = default;
	};
}