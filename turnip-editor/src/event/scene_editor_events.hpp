#pragma once
#include <turnip_engine.hpp>
#include "event/editor_events.hpp"

class SceneEditorResized : public tur::Event
{
	DEFINE_EVENT(EditorEvents::SCENE_EDITOR_RESIZED);

public:
	SceneEditorResized(tur::u32 width, tur::u32 height)
		: width(width)
		, height(height)
	{
	}

public:
	tur::u32 width;
	tur::u32 height;
};

class SceneEditorMoved : public tur::Event
{
	DEFINE_EVENT(EditorEvents::SCENE_EDITOR_MOVED);

public:
	SceneEditorMoved(tur::u32 x, tur::u32 y)
		: x(x)
		, y(y)
	{
	}

public:
	tur::u32 x;
	tur::u32 y;
};