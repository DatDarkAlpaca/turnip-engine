#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include "common.hpp"

namespace tur
{
	struct ScriptingInfo
	{
		std::string monoSDKvariable = "MONO_SDK";
		std::string assembliesPath = "mono/lib";
		std::string domainName = "TurScript";
		std::string monoVersion = "v4.0.30319";

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(
			ScriptingInfo,
			monoSDKvariable,
			assembliesPath,
			domainName,
			monoVersion
		);
	};
}