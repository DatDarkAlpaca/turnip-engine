#pragma once
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "core/logger/logger.hpp"

namespace tur
{
	inline std::string read_file(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath);
		if (!file.is_open())
			TUR_LOG_ERROR("Failed to load file: {}", filepath.string().c_str());

		std::stringstream ss;
		ss << file.rdbuf();

		return ss.str();
	}
}