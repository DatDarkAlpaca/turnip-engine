#include "pch.hpp"
#include "script_system.hpp"
#include "script_internals.hpp"

#include "core/engine/engine.hpp"

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

		mono_set_assemblies_path(assembliesFullPath.string().c_str());

		s_Domain = mono_jit_init_version(scriptData.domainName.c_str(), scriptData.monoVersion.c_str());
		TUR_ASSERT(s_Domain, "Failed to initialize JIT");

		mono_domain_set(s_Domain, false);
		mono_thread_set_main(mono_thread_current());
	}
	void ScriptSystem::load_assembly(const std::filesystem::path& filepath)
	{
		s_LoadedAssembly = mono_domain_assembly_open(s_Domain, "C:\\Users\\paulo\\Projects\\turnip-engine\\build\\bin\\Debug-windows-x86_64\\turnip-script\\turnip-script.dll");
		s_LoadedImage = mono_assembly_get_image(s_LoadedAssembly);
	}

	void ScriptSystem::on_scene_runtime_start(Scene* scene)
	{
		s_Scene = scene;

		register_internal_calls();

		populate_script_components();
	}
	void ScriptSystem::on_scene_runtime_update()
	{
		for (const auto& [entity, scripts] : s_Scene->get_registry().view<EntityScriptsComponent>().each())
		{
			for (const auto& script : scripts.scriptComponents)
			{
				if (!script.scriptData.instance)
					populate_script_components();

				mono_runtime_invoke(script.scriptData.updateMethod, script.scriptData.instance, nullptr, nullptr);
			}
		}
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

		mono_add_internal_call("TurnipScript.Internal::GetComponent_Native", get_component_native);

		mono_add_internal_call("TurnipScript.Internal::GetTransformPosition_Native", get_transform_position_native);
		mono_add_internal_call("TurnipScript.Internal::SetTransformPosition_Native", set_transform_position_native);
	}

	void ScriptSystem::populate_script_components()
	{
		for (auto& [entity, scripts] : s_Scene->get_registry().view<EntityScriptsComponent>().each())
		{
			for (auto& script : scripts.scriptComponents)
				instantiate_entity_object(script.className, entity, script.scriptData);
		}
	}

	void ScriptSystem::instantiate_entity_object(const std::string& className, entt::entity entityID, EntityScriptData& data)
	{
		MonoClass* entityClass = mono_class_from_name(s_LoadedImage, "TurnipScript", className.c_str());
		data.instance = mono_object_new(s_Domain, entityClass);

		mono_runtime_object_init(data.instance);

		// Sets entity ID:
		MonoClassField* field = mono_class_get_field_from_name(entityClass, "_id");
		mono_field_set_value(data.instance, field, &entityID);

		// TODO: implement in the initialize calls
		// ** Requires scene manager
		// Scene methods:
		{
			std::string sceneStartMethodName = className + "::OnSceneStart";
			MonoMethodDesc* sceneStartMethodDesc = mono_method_desc_new(sceneStartMethodName.c_str(), false);
			TUR_ASS(sceneStartMethodDesc);

			data.sceneStartMethod = mono_method_desc_search_in_class(sceneStartMethodDesc, entityClass);
			TUR_ASS(data.sceneStartMethod);

			std::string sceneEndMethodName = className + "::OnSceneEnd";
			MonoMethodDesc* sceneEndMethodDesc = mono_method_desc_new(sceneEndMethodName.c_str(), false);
			TUR_ASS(sceneEndMethodDesc);

			data.sceneEndMethod = mono_method_desc_search_in_class(sceneEndMethodDesc, entityClass);
			TUR_ASS(data.sceneEndMethod);
		}

		// Entity methods:
		{
			// TODO: implement
		}

		// Update method:
		{
			std::string updateMethodName = className + "::OnUpdate";
			MonoMethodDesc* updateDesc = mono_method_desc_new(updateMethodName.c_str(), false);
			TUR_ASS(updateDesc);

			data.updateMethod = mono_method_desc_search_in_class(updateDesc, entityClass);
			TUR_ASS(data.updateMethod);
		}
	}
}