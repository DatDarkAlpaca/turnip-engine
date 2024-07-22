#pragma once
#include <numeric>
#include <string>

namespace tur
{
	struct ApplicationSpecification
	{
		std::string applicationName = "Default Application";
		uint32_t versionMajor = 1, versionMinor = 0, versionPatch = 0, versionVariant = 0;
	};
}