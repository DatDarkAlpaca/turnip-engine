#pragma once
#include <filesystem>
#include <string>

#include <platform/platform.hpp>

namespace tur
{
	inline void create_solution(const std::string& projectName, const std::filesystem::path& projectFolder)
	{
		std::string projectFilepath = (projectFolder / projectName).string();
		run_command("dotnet new sln -n " + projectName + " -o " + projectFolder.string());
		run_command("dotnet sln " + projectFilepath + ".sln " + "add " + projectFilepath + ".csproj");
	}

	inline void compile_solution(const std::string& projectName, const std::filesystem::path& projectFolder)
	{
		std::string projectFilepath = (projectFolder / projectName).string();
		std::string binFolder = (projectFolder / "bin").string();
		run_command("dotnet build -o " + binFolder + " + " + projectFilepath + ".csproj");
	}
}