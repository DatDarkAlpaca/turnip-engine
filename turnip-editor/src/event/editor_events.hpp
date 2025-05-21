#pragma once
#include <turnip_engine.hpp>

enum class EditorEvents : u64
{
	ON_EDITOR_INIT = tur::NextAvailableEventID,

	ON_SCENE_EDITOR_RESIZE,
};