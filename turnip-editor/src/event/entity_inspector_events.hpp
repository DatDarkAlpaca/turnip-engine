#pragma once
#include <turnip_engine.hpp>
#include "editor_events.hpp"

class OnEntityInspectorInitialize : public tur::Event
{
	DEFINE_EVENT(EditorEvents::ON_EDITOR_INIT);

public:
	OnEntityInspectorInitialize() = default;
};