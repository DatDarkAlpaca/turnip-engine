#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <mono/jit/jit.h>

namespace tur
{
	struct EntityScriptData
	{
		MonoObject* instance = nullptr;

		MonoMethod* sceneStartMethod = nullptr;
		MonoMethod* sceneEndMethod = nullptr;
		MonoMethod* entityInstanceCreatedMethod = nullptr;
		MonoMethod* entityInstanceDestroyedMethod = nullptr;
		MonoMethod* updateMethod = nullptr;
	};

	struct InternalEntityScript
	{
	public:
		InternalEntityScript() = default;
		InternalEntityScript(const InternalEntityScript&) = default;
		InternalEntityScript(const std::string& className, const std::filesystem::path& filepath) 
			: className(className) 
			, filepath(filepath)
		{
		}

	public:
		std::string className;
		std::filesystem::path filepath;
		EntityScriptData scriptData;
	};

	struct EntityScriptsComponent
	{
	public:
		EntityScriptsComponent() = default;
		EntityScriptsComponent(const EntityScriptsComponent&) = default;

	public:
		void add(const InternalEntityScript& script)
		{
			scriptComponents.push_back(script);
		}

	public:
		std::vector<InternalEntityScript> scriptComponents;
	};
}