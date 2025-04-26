#include "pch.hpp"
#include "script_system.hpp"

#include "core/engine/engine.hpp"
#include "script_internals.hpp"

namespace tur
{
	void ScriptSystem::initialize(const ConfigData& configData, NON_OWNING TurnipEngine* engine)
	{
		using namespace std::filesystem;
		const auto& scriptData = configData.scriptingInfo;

		r_Engine = engine;

		path sdkVariable = std::getenv(scriptData.monoSDKvariable.c_str());
		path assembliesFullPath = sdkVariable / scriptData.assembliesPath;
		path configFullPath = sdkVariable / scriptData.configPath;
		mono_set_dirs(assembliesFullPath.string().c_str(), configFullPath.string().c_str());

		s_Domain = mono_jit_init_version(scriptData.domainName.c_str(), scriptData.monoVersion.c_str());
		mono_domain_set(s_Domain, false);
	}

	void ScriptSystem::on_scene_play(Scene* scene)
	{
		s_Scene = scene;

		s_LoadedAssembly = mono_domain_assembly_open(s_Domain, "turnip-script.dll");
		s_LoadedImage = mono_assembly_get_image(s_LoadedAssembly);

		register_internal_calls();

		MonoClass* entityClass = mono_class_from_name(s_LoadedImage, "TurnipScript", "Test");
		MonoObject* entityInstance = mono_object_new(s_Domain, entityClass);

		mono_runtime_object_init(entityInstance);

		// Sets entity ID:
		{
			MonoClassField* field = mono_class_get_field_from_name(entityClass, "_id");
			uint32_t id = 0;
			mono_field_set_value(entityInstance, field, &id);
		}

		// Calls OnUpdate:
		MonoMethodDesc* desc = mono_method_desc_new("Test::OnUpdate", false);
		MonoMethod* m = mono_method_desc_search_in_class(desc, entityClass);
		mono_runtime_invoke(m, entityInstance, nullptr, nullptr);
	}

	void ScriptSystem::register_internal_calls()
	{
		// Logging:
		mono_add_internal_call("TurnipScript.Internal::LogInfo", log_info);
		mono_add_internal_call("TurnipScript.Internal::LogTrace", log_trace);
		mono_add_internal_call("TurnipScript.Internal::LogDebug", log_debug);
		mono_add_internal_call("TurnipScript.Internal::LogWarn", log_warn);
		mono_add_internal_call("TurnipScript.Internal::LogError", log_error);
		mono_add_internal_call("TurnipScript.Internal::LogCritical", log_critical);

		mono_add_internal_call("TurnipScript.Internal::GetComponent_InternalNative", get_component_internal_native);
	}
}

