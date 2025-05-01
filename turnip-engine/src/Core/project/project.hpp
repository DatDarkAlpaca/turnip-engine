#pragma once
#include <filesystem>
#include <optional>
#include <fstream>

#include "common.hpp"
#include "utils/json/json_reader.hpp"
#include "utils/json/json_writer.hpp"

#include "core/script/script_compiler.hpp"

namespace tur
{
	struct ProjectData
	{
	public:
		std::string projectName;
		std::filesystem::path projectPath;

	public:
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(
			ProjectData,
			projectName,
			projectPath
		);
	};

	struct ProjectOptions
	{
		std::string projectName;
		std::filesystem::path projectFolder;
		std::filesystem::path domainFilepath;
	};

	inline std::optional<ProjectData> create_empty_project(const ProjectOptions& options)
	{
		using namespace std::filesystem;

		if (!exists(options.projectFolder) || !is_directory(options.projectFolder))
		{
			TUR_LOG_WARN("{} is not a valid directory. Failed to create empty project", options.projectFolder.string());
			return std::nullopt;
		}
	
		path binPath = options.projectFolder / "bin";
		create_directory(binPath);
		create_directory(options.projectFolder / "assets");

		// copies turnip-script.dll to bin/ and create a solution
		copy_file(options.domainFilepath, binPath / "turnip-script.dll", std::filesystem::copy_options::overwrite_existing);
		create_solution(options.projectName, options.projectFolder);

		ProjectData projectData;
		{
			projectData.projectName = options.projectName;
			projectData.projectPath = options.projectFolder;
		}

		std::string filename = options.projectName + ".json";
		JsonWriter writer(options.projectFolder / filename);
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