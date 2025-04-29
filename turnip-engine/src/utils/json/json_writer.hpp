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
		JsonWriter(const std::filesystem::path& filepath)
			: m_Filepath(filepath)
		{
		}

		JsonWriter() = default;

	public:
		void set_filepath(const std::filesystem::path& filepath)
		{
			m_Filepath = filepath;
		}

	public:
		template <typename DataType>
		void write(const DataType& type) const
		{
			nlohmann::json json{ type };
			
			std::ofstream jsonFile(m_Filepath);
			jsonFile << std::setfill(' ') << std::setw(4) << json[0];
		}

		void write(nlohmann::json jsonObject) const
		{
			std::ofstream jsonFile(m_Filepath);
			jsonFile << std::setfill(' ') << std::setw(4) << jsonObject;
		}

	private:
		std::filesystem::path m_Filepath;
	};
}