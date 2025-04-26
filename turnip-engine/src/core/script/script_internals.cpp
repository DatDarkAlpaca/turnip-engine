#include "pch.hpp"
#include "script_internals.hpp"

#include "core/script/script_system.hpp"
#include "core/scene/common_components.hpp"

void log_info(MonoString* message)
{
	TUR_LOG_INFO(mono_string_to_utf8(message));
}
void log_trace(MonoString* message)
{
	TUR_LOG_TRACE(mono_string_to_utf8(message));
}
void log_debug(MonoString* message)
{
	TUR_LOG_DEBUG(mono_string_to_utf8(message));
}
void log_warn(MonoString* message)
{
	TUR_LOG_WARN(mono_string_to_utf8(message));
}
void log_error(MonoString* message)
{
	TUR_LOG_ERROR(mono_string_to_utf8(message));
}
void log_critical(MonoString* message)
{
	TUR_LOG_CRITICAL(mono_string_to_utf8(message));
}

MonoObject* get_component_internal_native(tur::u32 entityID, MonoReflectionType* reflectionType)
{
	using namespace tur;

	MonoType* monoType = mono_reflection_type_get_type(reflectionType);
	const char* typeName = mono_type_get_name(monoType);

	// Transform:
	if (strcmp(typeName, "TurnipScript.Transform") == 0)
	{
		MonoClass* transformClass = mono_class_from_name(ScriptSystem::s_LoadedImage, "TurnipScript", "Transform");
		MonoObject* transformObject = mono_object_new(ScriptSystem::s_Domain, transformClass);

		void* transformUnboxed = mono_object_unbox(transformObject);

		auto transform = ScriptSystem::s_Scene->get_registry().get<TransformComponent>(static_cast<entt::entity>(entityID)).transform;
		memcpy(transformUnboxed, &transform, sizeof(Transform));

		return transformObject;
	}
}
