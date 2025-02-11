#pragma once
#include "graphics/graphics_specification.hpp"

namespace tur
{
	inline void to_json(nlohmann::json& json, const GraphicsSpecification& specification)
	{
		json["api"] = get_graphics_api_name(specification.api);
		json["major"] = specification.major;
		json["minor"] = specification.minor;
		json["patch"] = specification.patch;
		json["variant"] = specification.variant;
	}

	inline void from_json(const nlohmann::json& json, GraphicsSpecification& specification)
	{
		specification.api = get_graphics_api_from_string(json.at("api"));
		specification.major = json.at("major");
		specification.minor = json.at("minor");
		specification.patch = json.at("patch");
		specification.variant = json.at("variant");
	}
}