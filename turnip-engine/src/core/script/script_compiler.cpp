#include "pch.hpp"
#include "script_compiler.hpp"

static void create_project_csproj_file(const std::string& projectName, const std::filesystem::path& projectFolder)
{
	std::string fileContents = R"(
<Project Sdk="Microsoft.NET.Sdk">
	<PropertyGroup>
		<OutputType>Library</OutputType>
		<OutputPath>./bin/</OutputPath>
		<TargetFramework>net6.0</TargetFramework>
		<IsPackable>false</IsPackable>
	</PropertyGroup>
	<ItemGroup>
		<Reference Include="TurnipScript">
			<HintPath>./bin/turnip-script.dll</HintPath>
		</Reference>
	</ItemGroup>
</Project>
	)";

	std::string csFilepath = projectName + ".csproj";
	std::ofstream scriptFile(projectFolder / csFilepath);
	scriptFile << fileContents << '\n';

	scriptFile.close();
}

namespace tur 
{
	void create_solution(const std::string& projectName, const std::filesystem::path& projectFolder)
	{
		std::string projectFilepath = (projectFolder / projectName).string();

		if (!std::filesystem::exists(projectFilepath + ".csproj"))
			create_project_csproj_file(projectName, projectFolder);

		run_command("dotnet new sln -n " + projectName + " -o " + projectFolder.string());
		run_command("dotnet sln " + projectFilepath + ".sln " + "add " + projectFilepath + ".csproj");
	}

	void compile_solution(const std::string& projectName, const std::filesystem::path& projectFolder)
	{
		std::string projectFilepath = (projectFolder / projectName).string() + ".sln";

		if (!std::filesystem::exists(projectFilepath + ".csproj"))
			create_project_csproj_file(projectName, projectFolder);

		if (!std::filesystem::exists(projectFilepath))
			create_solution(projectName, projectFolder);

		std::string binFolder = (projectFolder / "bin").string();
		run_command("dotnet build " + projectFilepath + " -o " + binFolder);
	}
}