#pragma once
#include <mono/jit/jit.h>
#include "common.hpp"

extern "C"
{
	void log_info(MonoString* message);
	void log_trace(MonoString* message);
	void log_debug(MonoString* message);
	void log_warn(MonoString* message);
	void log_error(MonoString* message);
	void log_critical(MonoString* message);
}

extern "C"
{
	MonoObject* get_component_native(tur::u32 entityID, MonoReflectionType* reflectionType);

	void get_transform_position_native(tur::u32 entityID, glm::vec3* outPosition);
	void set_transform_position_native(tur::u32 entityID, glm::vec3* inPosition);

	void get_transform_rotation_native(tur::u32 entityID, glm::vec3* outRotation);
	void set_transform_rotation_native(tur::u32 entityID, glm::vec3* inRotation);

	void get_transform_scale_native(tur::u32 entityID, glm::vec3* outScale);
	void set_transform_scale_native(tur::u32 entityID, glm::vec3* inScale);

	struct PlainVec2D { float x; float y; };
	PlainVec2D get_mouse_position_window_native();
}
