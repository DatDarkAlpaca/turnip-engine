#pragma once
#include <fstream>
#include <sstream>
#include <string>

#include "Core/Logger/Logger.h"

namespace tur
{
	inline std::string ReadFile(const std::string& filepath)
	{
		std::ifstream file(filepath, std::ios::binary | std::ios::ate);
		if (!file.is_open())
			TUR_LOG_ERROR("Failed to load file: {}", filepath);

		std::stringstream ss;

		ss << file.rdbuf();

		return ss.str();
	}
}