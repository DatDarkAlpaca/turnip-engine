#pragma once
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#include "core/config/config_data.hpp"
#include "core/event/event.hpp"
#include "common.hpp"

namespace tur
{
	class TurnipEngine;

	class ScriptSystem
	{
	public:
		void initialize(const ConfigData& configData, NON_OWNING TurnipEngine* engine);

		void shutdown();

	public:
		void load(const std::filesystem::path& filepath, const std::string& className);

	public:	
		void on_update();

	private:
		MonoMethod* get_method(MonoImage* image, const std::string& methodName) const;

		void register_internal_calls();

	private:
		NON_OWNING TurnipEngine* r_Engine = nullptr;
		MonoDomain* m_Domain = nullptr;

	private:
		struct ScriptMethods
		{
			MonoMethod* onLoadMethod;
			MonoMethod* updateMethod;
		};

		std::vector<ScriptMethods> m_ScriptMethods;
	};
}