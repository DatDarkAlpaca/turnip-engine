#pragma once
#include <filesystem>
#include <optional>
#include <fstream>

#include "common.hpp"

#include "core/assets/asset_library.hpp"
#include "core/script/script_compiler.hpp"

namespace tur
{
	struct AssetMetaData
	{
		std::filesystem::path filepath;
		asset_handle assetHandle = invalid_handle;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(
			AssetMetaData,
			filepath,
			assetHandle
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

	std::optional<ProjectData> read_project_file(const std::filesystem::path& filepath);
}