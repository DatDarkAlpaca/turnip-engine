#pragma once
#include <turnip_engine.hpp>
#include "editor_events.hpp"

class OnProjectEdited : public tur::Event
{
	DEFINE_EVENT(EditorEvents::PROJECT_EDITED);

public:
	OnProjectEdited() = default;
};

class OnProjectSaved : public tur::Event
{
	DEFINE_EVENT(EditorEvents::PROJECT_SAVED);

public:
	OnProjectSaved() = default;
};