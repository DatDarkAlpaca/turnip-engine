#pragma once
#include <turnip_engine.hpp>
#include "event/editor_events.hpp"

class SceneEditorResize : public tur::Event
{
	DEFINE_EVENT(EditorEvents::ON_SCENE_EDITOR_RESIZE);

public:
	SceneEditorResize(tur::u32 width, tur::u32 height)
		: width(width)
		, height(height)
	{
	}

public:
	tur::u32 width;
	tur::u32 height;
};