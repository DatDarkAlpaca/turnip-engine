#pragma once
#include <turnip_engine.hpp>

enum class EditorEvents : u64
{
	ON_EDITOR_INIT = tur::NextAvailableEventID,

	SCENE_EDITOR_RESIZED,
	SCENE_EDITOR_MOVED
};