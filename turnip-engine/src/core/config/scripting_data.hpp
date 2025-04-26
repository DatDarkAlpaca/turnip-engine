#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include "common.hpp"

namespace tur
{
	struct ScriptingInfo
	{
		std::string monoSDKvariable = "MONO_SDK";
		std::string monoAssembliesPath = "lib";
		std::string domainName = "TurScript";
		std::string monoConfigPath = "etc";
		std::string monoVersion = "v4.0.30319";

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(
			ScriptingInfo,
			monoSDKvariable,
			monoAssembliesPath,
			monoConfigPath,
			domainName,
			monoVersion
		);
	};
}