#pragma once
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#include "core/config/config_data.hpp"
#include "core/project/project.hpp"
#include "core/scene/components.hpp"
#include "core/scene/scene.hpp"
#include "core/event/event.hpp"
#include "common.hpp"

namespace tur
{
	class TurnipEngine;

	class ScriptSystem
	{
	public:
		static void initialize(const ConfigData& configData, NON_OWNING TurnipEngine* engine);
		
		static void load_domain();
		static void load_assembly();
		static void load_user_assembly();

		static void set_project(const ProjectData& projectData);

	public:
		static void on_scene_runtime_start(Scene* scene);
		static void on_scene_runtime_update();

	private:
		static void reload_system();
		
		static void register_internal_calls();

	private:
		static void populate_script_components();

		static void instantiate_entity_object(const std::string& className, entt::entity entityID, EntityScriptData& data);

	private:
		static inline NON_OWNING TurnipEngine* r_Engine = nullptr;

	public:
		static inline MonoDomain* s_Domain = nullptr;

		static inline MonoAssembly* s_LoadedAssembly = nullptr;
		static inline MonoAssembly* s_UserAssembly = nullptr;
		static inline MonoImage* s_LoadedImage = nullptr;
		static inline MonoImage* s_UserImage = nullptr;

		static inline MonoClass* s_InternalsClass = nullptr;
		static inline tur::Scene* s_Scene = nullptr;

	private:
		static inline ConfigData s_ConfigData;
		static inline ProjectData s_ProjectData;
	};
}