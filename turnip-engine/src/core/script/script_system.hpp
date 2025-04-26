#pragma once
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#include "core/config/config_data.hpp"
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

	public:
		static void on_scene_play(Scene* scene);

	private:
		static void register_internal_calls();

	private:
		static inline NON_OWNING TurnipEngine* r_Engine = nullptr;

	public:
		static inline MonoDomain* s_Domain = nullptr;
		static inline MonoAssembly* s_LoadedAssembly = nullptr;
		static inline MonoImage* s_LoadedImage = nullptr;

		static inline MonoClass* s_InternalsClass = nullptr;
		static inline tur::Scene* s_Scene = nullptr;
	};
}