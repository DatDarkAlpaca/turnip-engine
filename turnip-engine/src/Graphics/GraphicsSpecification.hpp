#pragma once
#include <nlohmann/json.hpp>
#include <numeric>

#include "GraphicsAPI.hpp"

namespace tur
{
	struct GraphicsSpecification
	{
		GraphicsAPI api = GraphicsAPI::UNKNOWN;
		uint8_t versionMajor = 0, versionMinor = 0, versionPatch = 0, versionVariant = 0;
	};

	inline void to_json(nlohmann::json& json, const GraphicsSpecification& specification)
	{
		json["api"] = GetGraphicsAPIString(specification.api);
		json["major"] = specification.versionMajor;
		json["minor"] = specification.versionMinor;
		json["patch"] = specification.versionPatch;
		json["variant"] = specification.versionVariant;
	}

	inline void from_json(const nlohmann::json& json, GraphicsSpecification& specification)
	{
		specification.api = GetGraphicsAPIFromString(json.at("api"));
		specification.versionMajor = json.at("versionMajor");
		specification.versionMinor = json.at("versionMinor");
		specification.versionPatch = json.at("versionPatch");
		specification.versionVariant = json.at("versionVariant");
	}
}