#pragma once
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>

#include "utils/json/json_file.hpp"
#include "common.hpp"

namespace tur
{
	class JsonWriter
	{
	public:
		JsonWriter(const std::filesystem::path& filePath)
			: m_Filepath(filePath)
		{
		}

	public:
		template <typename DataType>
		void write(const DataType& type)
		{
			nlohmann::json json{ type };
			
			std::ofstream jsonFile(m_Filepath);
			jsonFile << std::setfill(' ') << std::setw(4) << json[0];
		}

	private:
		std::filesystem::path m_Filepath;
	};
}