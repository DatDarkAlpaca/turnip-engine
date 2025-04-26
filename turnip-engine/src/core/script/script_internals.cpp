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

MonoObject* get_component_native(tur::u32 entityID, MonoReflectionType* reflectionType)
{
	using namespace tur;

	MonoType* monoType = mono_reflection_type_get_type(reflectionType);
	const char* typeName = mono_type_get_name(monoType);

	// Transform:
	if (strcmp(typeName, "TurnipScript.Transform") == 0)
	{
		MonoClass* transformClass = mono_class_from_name(ScriptSystem::s_LoadedImage, "TurnipScript", "Transform");
		MonoObject* transformObject = mono_object_new(ScriptSystem::s_Domain, transformClass);

		MonoMethod* constructor = mono_class_get_method_from_name(transformClass, ".ctor", 1);

		void* args[1] = { &entityID };
		mono_runtime_invoke(constructor, transformObject, args, nullptr);

		return transformObject;
	}

	return nullptr;
}

void get_transform_position_native(tur::u32 entityID, glm::vec3* outPosition)
{
	using namespace tur;

	const auto& transform = ScriptSystem::s_Scene->get_registry().get<TransformComponent>(static_cast<entt::entity>(entityID)).transform;
	*outPosition = transform.position;
}
void set_transform_position_native(tur::u32 entityID, glm::vec3* inPosition)
{
	using namespace tur;

	auto& transform = ScriptSystem::s_Scene->get_registry().get<TransformComponent>(static_cast<entt::entity>(entityID)).transform;
	transform.position = *inPosition;
}