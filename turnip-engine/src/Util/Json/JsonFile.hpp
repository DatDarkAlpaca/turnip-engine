#pragma once
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>

namespace loaf
{
	inline nlohmann::json readJson(const std::filesystem::path& filepath)
	{
		using namespace nlohmann;

		std::ifstream file(filepath);
		return json::parse(file);
	}
}