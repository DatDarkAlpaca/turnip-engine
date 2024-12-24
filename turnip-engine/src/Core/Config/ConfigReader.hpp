#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>

#include "Utils/Json/JsonFile.hpp"
#include "Common.hpp"

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
		DataType Parse()
		{
			nlohmann::json jsonObject = ReadJson(m_Filepath);
			return jsonObject.template get<DataType>();
		}

	private:
		std::filesystem::path m_Filepath;
	};
}