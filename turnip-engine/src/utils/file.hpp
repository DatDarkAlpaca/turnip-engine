#pragma once
#include <portable-file-dialogs.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "core/logger/logger.hpp"
#include "common.hpp"

namespace tur
{
	// TODO: exists filepath
	inline std::string read_file(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath);
		if (!file.is_open())
			TUR_LOG_ERROR("Failed to load file: {}", filepath.string().c_str());

		std::stringstream ss;
		ss << file.rdbuf();

		return ss.str();
	}

	inline std::vector<u8> read_file_binary(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath, std::ios::binary | std::ios::ate);
		if (!file.is_open())
			TUR_LOG_ERROR("Failed to load file: {}", filepath.string().c_str());

		u64 fileSize = static_cast<u64>(file.tellg());
		std::vector<u8> data(fileSize);

		file.seekg(0);
		file.read(reinterpret_cast<char*>(data.data()), fileSize);

		file.close();

		return data;
	}

	inline std::string save_file_dialog(const std::string& title, const std::vector<std::string>& filters = {})
	{
		return pfd::save_file::save_file(title, "", filters).result();
	}

	inline std::vector<std::string> open_file_dialog(const std::string& title, const std::vector<std::string>& filters = {}, bool allowMulti = false)
	{
		if (!allowMulti)
			return pfd::open_file::open_file(title, "", filters).result();
		else
			return pfd::open_file::open_file(title, "", filters, pfd::opt::multiselect).result();
	}

	inline std::string open_folder_dialog(const std::string& title)
	{
		return pfd::select_folder::select_folder(title, "").result();
	}
}