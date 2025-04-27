#pragma once
#include <filesystem>
#include <optional>

#include "common.hpp"
#include "utils/json/json_reader.hpp"
#include "utils/json/json_writer.hpp"

namespace tur
{
	struct ProjectData
	{
		std::string projectName;
		std::filesystem::path projectPath;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(
			ProjectData,
			projectName,
			projectPath
		);
	};

	inline std::optional<ProjectData> create_empty_project(std::string projectName, const std::filesystem::path& folderPath)
	{
		if (!std::filesystem::exists(folderPath) || !std::filesystem::is_directory(folderPath))
		{
			TUR_LOG_WARN("{} is not a valid directory. Failed to create empty project", folderPath.string());
			return std::nullopt;
		}
	
		std::filesystem::create_directory(folderPath / "bin");
		std::filesystem::create_directory(folderPath / "assets");	

		ProjectData projectData;
		{
			projectData.projectName = projectName;
			projectData.projectPath = folderPath;
		}

		std::string filename = projectName + ".json";
		JsonWriter writer(folderPath / filename);
		writer.write<ProjectData>(projectData);

		return projectData;
	}

	inline std::optional<ProjectData> read_project_file(const std::filesystem::path& filepath)
	{
		if (!std::filesystem::exists(filepath) || !std::filesystem::is_regular_file(filepath))
		{
			TUR_LOG_WARN("{} is not a valid project file.", filepath.string());
			return std::nullopt;
		}

		JsonReader reader(filepath);
		return reader.parse<ProjectData>();
	}
}