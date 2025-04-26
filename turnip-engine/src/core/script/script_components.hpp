#pragma once
#include <string>
#include <vector>
#include <mono/jit/jit.h>

namespace tur
{
	struct InternalEntityScript
	{
	public:
		InternalEntityScript() = default;
		InternalEntityScript(const InternalEntityScript&) = default;
		InternalEntityScript(const std::string& className) : className(className) {}

	public:
		std::string className;
		MonoObject* instance = nullptr;
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