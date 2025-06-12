#pragma once
#include <filesystem>
#include <optional>
#include <fstream>
#include <nlohmann/json.hpp>

#include "common.hpp"
#include "core/event/event.hpp"
#include "core/worker/worker_pool.hpp"
#include "core/assets/asset_library.hpp"

namespace tur
{
	struct AssetMetaData
	{
		std::filesystem::path filepath;
		UUID uuid;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(
			AssetMetaData,
			filepath,
			uuid
		);
	};

	struct ProjectData
	{
	public:
		std::string projectName;
		std::filesystem::path projectPath;
		std::vector<AssetMetaData> assetMetadata;

	public:
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(
			ProjectData,
			projectName,
			projectPath,
			assetMetadata
		);
	};

	struct ProjectOptions
	{
		std::string projectName;
		std::filesystem::path projectFolder;
		std::filesystem::path domainFilepath;
	};

	std::optional<ProjectData> create_empty_project(const ProjectOptions& options);

	void save_project_data(ProjectData& data, AssetLibrary* assetLibrary);

	void load_project_data(ProjectData& data, AssetLibrary* assetLibrary, WorkerPool* pool, EventCallback&& callback);

	std::optional<ProjectData> read_project_file(const std::filesystem::path& filepath);
}