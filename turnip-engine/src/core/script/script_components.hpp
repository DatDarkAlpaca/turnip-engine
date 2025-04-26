#pragma once
#include <string>
#include <vector>
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
		InternalEntityScript(const std::string& className) : className(className) {}

	public:
		std::string className;
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