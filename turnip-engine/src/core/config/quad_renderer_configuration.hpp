#pragma once
#include <nlohmann/json.hpp>
#include "common.hpp"
#include "graphics/renderer/quad_renderer_data.hpp"

namespace tur
{
	inline void to_json(nlohmann::json& json, const QuadRendererInformation& quadInfo)
	{
		json["vertexFilepath"] = quadInfo.vertexFilepath;
		json["fragmentFilepath"] = quadInfo.fragmentFilepath;
	}

	inline void from_json(const nlohmann::json& json, QuadRendererInformation& quadInfo)
	{
		quadInfo.vertexFilepath = json["vertexFilepath"];
		quadInfo.fragmentFilepath = json["fragmentFilepath"];
	}
}