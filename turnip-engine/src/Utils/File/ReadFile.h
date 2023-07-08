#pragma once
#include "pch.h"
#include "Logger/Logger.h"

// Todo: improve
namespace tur
{
	inline std::string ReadFile(const char* filepath)
	{
		FILE* file = nullptr;
		fopen_s(&file, filepath, "r");

		if (!file)
		{
			TUR_CORE_ERROR("Failed to open file: {}", filepath);
			return {};
		}

		std::string result;
		
		char buffer[1024];
		while (fgets(buffer, 1024, file))
			result += buffer;

		return result;
	}
}