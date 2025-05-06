#include "pch.hpp"
#include "project.hpp"

namespace tur
{
	std::optional<ProjectData> tur::create_empty_project(const ProjectOptions& options)
	{
		using namespace std::filesystem;

		if (!exists(options.projectFolder) || !is_directory(options.projectFolder))
		{
			TUR_LOG_WARN("{} is not a valid directory. Failed to create empty project", options.projectFolder.string());
			return std::nullopt;
		}

		path binPath = options.projectFolder / TUR_BIN_FOLDERNAME;
		create_directory(binPath);
		create_directory(options.projectFolder / TUR_ASSET_FOLDERNAME);

		// copies turnip-script.dll to bin/ and create a solution
		copy_file(options.domainFilepath, binPath / TUR_SCRIPT_DLL_FILENAME, std::filesystem::copy_options::overwrite_existing);
		create_solution(options.projectName, options.projectFolder);

		ProjectData projectData;
		{
			projectData.projectName = options.projectName;
			projectData.projectPath = options.projectFolder;
		}

		std::string filename = options.projectName + TUR_ENGINE_FILE_EXTENSION;
		JsonWriter writer(options.projectFolder / filename);
		writer.write<ProjectData>(projectData);

		return projectData;
	}

	std::optional<ProjectData> read_project_file(const std::filesystem::path& filepath)
	{
		if (!std::filesystem::exists(filepath) || !std::filesystem::is_regular_file(filepath))
		{
			TUR_LOG_WARN("{} is not a valid project file.", filepath.string());
			return std::nullopt;
		}

		JsonReader reader(filepath);
		auto parseResult = reader.parse<ProjectData>();
		if (!parseResult.has_value())
			return std::nullopt;

		return parseResult.value();
	}
}
