#pragma once
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>

namespace tur
{
	inline nlohmann::json read_json(const std::filesystem::path& filepath)
	{
		using namespace nlohmann;

		std::ifstream file(filepath);
		return nlohmann::json::parse(file);
	}
}