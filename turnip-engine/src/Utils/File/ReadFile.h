#pragma once
#include <string>
#include <stdio.h>
#include "Utils.h"

// Todo: use a platform layer and a proper filesytem
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