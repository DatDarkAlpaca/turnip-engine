#pragma once
#include <nlohmann/json.hpp>
#include <numeric>
#include <string>

namespace tur
{
	struct ApplicationSpecification
	{
		std::string applicationName = "Default Application";
		uint32_t versionMajor = 1, versionMinor = 0, versionPatch = 0, versionVariant = 0;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(
			ApplicationSpecification, 
			applicationName, 
			versionMajor, 
			versionMinor, 
			versionPatch, 
			versionVariant
		);
	};
}