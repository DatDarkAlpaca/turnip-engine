#pragma once
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>

#include "utils/json/json_file.hpp"
#include "common.hpp"

namespace tur
{
	class ConfigReader
	{
	public:
		ConfigReader(const std::filesystem::path& filePath)
			: m_Filepath(filePath)
		{
		}

	public:
		template <typename DataType>
		DataType parse()
		{
			nlohmann::json jsonObject = read_json(m_Filepath);
			return jsonObject.template get<DataType>();
		}

	private:
		std::filesystem::path m_Filepath;
	};
}