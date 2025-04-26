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
	MonoObject* get_component_internal_native(tur::u32 entityID, MonoReflectionType* reflectionType);
}
