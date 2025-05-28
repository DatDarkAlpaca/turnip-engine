#pragma once
#include <nlohmann/json.hpp>
#include "common.hpp"
#include "graphics/renderer/instanced/instanced_quad_renderer_data.hpp"

namespace tur
{
	inline void to_json(nlohmann::json& json, const InstancedQuadRendererInformation& quadInfo)
	{
		json["vertexFilepath"] = quadInfo.vertexFilepath;
		json["fragmentFilepath"] = quadInfo.fragmentFilepath;

		json["maxInstanceCount"] = quadInfo.maxInstanceCount;
		json["maxTextureCount"] = quadInfo.maxTextureCount;
		json["textureWidth"] = quadInfo.textureWidth;
		json["textureHeight"] = quadInfo.textureHeight;
	}

	inline void from_json(const nlohmann::json& json, InstancedQuadRendererInformation& quadInfo)
	{
		quadInfo.vertexFilepath = json["vertexFilepath"];
		quadInfo.fragmentFilepath = json["fragmentFilepath"];

		quadInfo.maxInstanceCount = json["maxInstanceCount"];
		quadInfo.maxTextureCount = json["maxTextureCount"];
		quadInfo.textureWidth = json["textureWidth"];
		quadInfo.textureHeight = json["textureHeight"];
	}
}