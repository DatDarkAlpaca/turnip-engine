#pragma once
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>

#include "utils/json/json_file.hpp"
#include "common.hpp"

namespace tur
{
	class JsonReader
	{
	public:
		JsonReader(const std::filesystem::path& filePath)
			: m_Filepath(filePath)
		{
		}

	public:
		template <typename DataType>
		DataType parse()
		{
			nlohmann::json jsonObject;
			try 
			{
				jsonObject = read_json(m_Filepath);
			}
			catch (nlohmann::json::exception& e)
			{
				TUR_LOG_CRITICAL("Failed to parse json file at: {}. {}", m_Filepath.string(), e.what());
			}

			DataType result;
			try
			{
				result = jsonObject.template get<DataType>();
			}
			catch (nlohmann::json::exception& e)
			{
				TUR_LOG_CRITICAL("Failed to get element in json file at: {}. {}", m_Filepath.string(), e.what());
			}
			
			return result;
		}

		nlohmann::json parse()
		{
			return read_json(m_Filepath);
		}

	private:
		std::filesystem::path m_Filepath;
	};
}