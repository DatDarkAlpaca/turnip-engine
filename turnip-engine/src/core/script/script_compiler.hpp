#pragma once
#include <filesystem>
#include <string>

#include <platform/platform.hpp>

namespace tur
{
	void create_solution(const std::string& projectName, const std::filesystem::path& projectFolder);

	void compile_solution(const std::string& projectName, const std::filesystem::path& projectFolder);

	void create_empty_script(const std::filesystem::path& filepath, const std::string className);
}