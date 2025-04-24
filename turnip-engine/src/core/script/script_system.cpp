#include "pch.hpp"
#include "script_system.hpp"
#include "core/engine/engine.hpp"

namespace tur
{
	void ScriptSystem::initialize(const ConfigData& configData, NON_OWNING TurnipEngine* engine)
	{
		using namespace std::filesystem;
		const auto& scriptData = configData.scriptingInfo;

		r_Engine = engine;
		configData.scriptingInfo;

		path sdkVariable = std::getenv(scriptData.monoSDKvariable.c_str());
		path assembliesPath = scriptData.assembliesPath;
		path fullPath = sdkVariable / assembliesPath;
		mono_set_assemblies_path(fullPath.string().c_str());
	
		m_Domain = mono_jit_init_version(scriptData.domainName.c_str(), scriptData.monoVersion.c_str());

		register_internal_calls();		
	}

	void ScriptSystem::shutdown()
	{
		mono_jit_cleanup(m_Domain);
		m_Domain = nullptr;
	}

	void ScriptSystem::load(const std::filesystem::path& filepath, const std::string& className)
	{
		MonoAssembly* assembly = mono_domain_assembly_open(m_Domain, filepath.string().c_str());
		if (!assembly)
			return TUR_LOG_ERROR("Failed to load assemblies from: {}", filepath.string());

		MonoImage* image = mono_assembly_get_image(assembly);
		
		ScriptMethods methods = {};
		methods.onLoadMethod        = get_method(image, className + ":OnLoadScript()");
		methods.updateMethod		= get_method(image, className + ":OnUpdate()");

		m_ScriptMethods.push_back(methods);
	}

	void ScriptSystem::on_update()
	{
		for (const auto& methods : m_ScriptMethods)
			mono_runtime_invoke(methods.updateMethod, nullptr, nullptr, nullptr);
	}

	MonoMethod* ScriptSystem::get_method(MonoImage* image, const std::string& methodName) const
	{
		MonoMethodDesc* methodDesc = mono_method_desc_new(methodName.c_str(), false);
		MonoMethod* method = mono_method_desc_search_in_image(methodDesc, image);
		mono_method_desc_free(methodDesc);

		return method;
	}
	
	void ScriptSystem::register_internal_calls()
	{
		r_Engine->get_window().data.properties;

		// mono_add_internal_call("Engine.Input::IsKeyPressed", (void*)IsKeyPressed);
	}
}
