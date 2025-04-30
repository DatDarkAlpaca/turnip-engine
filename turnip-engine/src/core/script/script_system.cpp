#include "pch.hpp"
#include "script_system.hpp"
#include "script_internals.hpp"

#include "core/engine/engine.hpp"
#include "script_compiler.hpp"

namespace tur
{
	void ScriptSystem::initialize(const ConfigData& configData, NON_OWNING TurnipEngine* engine)
	{
		using namespace std::filesystem;
		const auto& scriptData = configData.scriptingInfo;

		s_ConfigData = configData;
		r_Engine = engine;
		
		path sdkVariable = std::getenv(scriptData.monoSDKvariable.c_str());
		path assembliesFullPath = sdkVariable / scriptData.monoAssembliesPath;
		path configFullPath = sdkVariable / scriptData.monoConfigPath;
		mono_set_dirs(assembliesFullPath.string().c_str(), configFullPath.string().c_str());

		load_domain();

		load_app_domain();

		load_assembly();
	}

	void ScriptSystem::load_domain()
	{
		s_Domain = mono_jit_init_version(s_ConfigData.scriptingInfo.domainName.c_str(), s_ConfigData.scriptingInfo.monoVersion.c_str());
		TUR_ASSERT(s_Domain, "Failed to initialize JIT");

		mono_domain_set(s_Domain, false);
		mono_thread_set_main(mono_thread_current());
	}
	void ScriptSystem::load_app_domain()
	{
		s_AppDomain = mono_domain_create_appdomain("TurnipDomain", nullptr);
		mono_domain_set(s_AppDomain, false);
	}
	void ScriptSystem::load_assembly()
	{
		s_LoadedAssembly = mono_domain_assembly_open(s_AppDomain, s_ConfigData.scriptingInfo.mainDomainPath.c_str());
		s_LoadedImage = mono_assembly_get_image(s_LoadedAssembly);
	}
	void ScriptSystem::load_user_assembly()
	{
		using namespace std::filesystem;

		std::string dllName = s_ProjectData.projectName + ".dll";
		path assemblyPath = s_ProjectData.projectPath / path("bin") / dllName;

		s_UserAssembly = mono_domain_assembly_open(s_AppDomain, assemblyPath.string().c_str());
		s_UserImage = mono_assembly_get_image(s_UserAssembly);
	}

	void ScriptSystem::set_project(const ProjectData& projectData)
	{
		s_ProjectData = projectData;
	}

	void ScriptSystem::on_scene_runtime_start(Scene* scene)
	{
		using namespace std::filesystem;

		s_Scene = scene;

		compile_solution(s_ProjectData.projectName, s_ProjectData.projectPath);
	
		reload_system();

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

				if(script.scriptData.updateMethod)
					mono_runtime_invoke(script.scriptData.updateMethod, script.scriptData.instance, nullptr, nullptr);
			}
		}
	}

	void ScriptSystem::reload_system()
	{
		mono_domain_set(s_Domain, false);

		mono_domain_unload(s_AppDomain);
		load_app_domain();

		load_assembly();
		load_user_assembly();
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

		// Components:
		mono_add_internal_call("TurnipScript.Internal::GetComponent_Native", get_component_native);

		// Transform:
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
		MonoClass* entityClass = mono_class_from_name(s_UserImage, "TurnipScript", className.c_str());
		data.instance = mono_object_new(s_AppDomain, entityClass);

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

			std::string sceneEndMethodName = className + "::OnSceneEnd";
			MonoMethodDesc* sceneEndMethodDesc = mono_method_desc_new(sceneEndMethodName.c_str(), false);
			TUR_ASS(sceneEndMethodDesc);

			data.sceneEndMethod = mono_method_desc_search_in_class(sceneEndMethodDesc, entityClass);
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
		}
	}
}