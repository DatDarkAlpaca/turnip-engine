#include "pch.hpp"

#include "script_internals.hpp"

#include "platform/window.hpp"
#include "core/engine/engine.hpp"
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
	auto& registry = tur::ScriptSystem::s_Scene->get_registry();
	entt::entity entity = static_cast<entt::entity>(entityID);

	if (!registry.all_of<tur::TransformComponent>(entity))
		return TUR_LOG_WARN("Entity {} does not have a Transform component", entityID);

	*outPosition = registry.get<tur::TransformComponent>(entity).transform.position;
}
void set_transform_position_native(tur::u32 entityID, glm::vec3* inPosition)
{
	auto& registry = tur::ScriptSystem::s_Scene->get_registry();
	entt::entity entity = static_cast<entt::entity>(entityID);

	if (!registry.all_of<tur::TransformComponent>(entity))
		return TUR_LOG_WARN("Entity {} does not have a Transform component", entityID);

	auto& transform = registry.get<tur::TransformComponent>(entity).transform;
	transform.position = *inPosition;
}

void get_transform_rotation_native(tur::u32 entityID, glm::vec3* outRotation)
{
	auto& registry = tur::ScriptSystem::s_Scene->get_registry();
	entt::entity entity = static_cast<entt::entity>(entityID);

	if (!registry.all_of<tur::TransformComponent>(entity))
		return TUR_LOG_WARN("Entity {} does not have a Transform component", entityID);

	*outRotation = registry.get<tur::TransformComponent>(entity).transform.rotation;
}
void set_transform_rotation_native(tur::u32 entityID, glm::vec3* inRotation)
{
	auto& registry = tur::ScriptSystem::s_Scene->get_registry();
	entt::entity entity = static_cast<entt::entity>(entityID);

	if (!registry.all_of<tur::TransformComponent>(entity))
		return TUR_LOG_WARN("Entity {} does not have a Transform component", entityID);

	auto& transform = registry.get<tur::TransformComponent>(entity).transform;
	transform.rotation = *inRotation;
}

void get_transform_scale_native(tur::u32 entityID, glm::vec3* outScale)
{
	auto& registry = tur::ScriptSystem::s_Scene->get_registry();
	entt::entity entity = static_cast<entt::entity>(entityID);

	if (!registry.all_of<tur::TransformComponent>(entity))
		return TUR_LOG_WARN("Entity {} does not have a Transform component", entityID);

	*outScale = registry.get<tur::TransformComponent>(entity).transform.scale;
}
void set_transform_scale_native(tur::u32 entityID, glm::vec3* inScale)
{
	auto& registry = tur::ScriptSystem::s_Scene->get_registry();
	entt::entity entity = static_cast<entt::entity>(entityID);

	if (!registry.all_of<tur::TransformComponent>(entity))
		return TUR_LOG_WARN("Entity {} does not have a Transform component", entityID);

	auto& transform = registry.get<tur::TransformComponent>(entity).transform;
	transform.scale = *inScale;
}

PlainVec2D get_mouse_position_window_native()
{
	glm::vec2 position = tur::get_mouse_cursor_position_window(&tur::ScriptSystem::r_Engine->window);
	return { position.x, position.y };
}