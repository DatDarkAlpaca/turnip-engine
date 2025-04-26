#pragma once
#include <filesystem>

#include "common.hpp"

namespace tur
{
	inline void create_empty_project(const std::filesystem::path& folderPath)
	{
		if (!std::filesystem::exists(folderPath) || !std::filesystem::is_directory(folderPath))
			return TUR_LOG_WARN("{} is not a valid directory. Failed to create empty project", folderPath.string());

		std::filesystem::create_directory(folderPath / "bin");
	}
}