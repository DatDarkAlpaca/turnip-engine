#pragma once
#include <fstream>
#include <sstream>
#include <string>

#include "Core/Logger/Logger.hpp"

namespace tur
{
	inline std::string ReadFile(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (!file.is_open())
			TUR_LOG_ERROR("Failed to load file: {}", filepath);

		std::stringstream ss;

		ss << file.rdbuf();

		return ss.str();
	}

	inline std::vector<char> ReadBinaryFile(const std::string& filepath)
	{
		std::ifstream file(filepath, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			TUR_LOG_ERROR("Failed to load file: {}", filepath);

		size_t fileSize { static_cast<size_t>(file.tellg()) };

		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}
}