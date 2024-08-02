#pragma once
#include <filesystem>
#include "ConfigData.hpp"
#include "ConfigParser.hpp"

namespace tur
{
	class ConfigSystem
	{
	public:
		explicit ConfigSystem(const std::filesystem::path& configFilepath)
			: m_ConfigFilepath(configFilepath)
		{
			
		}

		ConfigSystem() = default;

	public:
		void Initialize(tur_unique<IConfigParser> configParser)
		{
			SetParser(std::move(configParser));

			if (!std::filesystem::exists(m_ConfigFilepath))
				Save();

			Load();
		}

	public:
		void Load()
		{
			m_Config = m_ConfigParser->Load(m_ConfigFilepath);
		}

		void Save()
		{
			m_ConfigParser->Save(m_Config, m_ConfigFilepath);
		}

	public:
		void SetFilepath(const std::filesystem::path& filepath)
		{
			m_ConfigFilepath = filepath;
		}

		void SetParser(tur_unique<IConfigParser> parser)
		{
			m_ConfigParser = std::move(parser);
		}

	public:
		const ConfigData& data() const { return m_Config; }

		ConfigData& data() { return m_Config; }

	private:
		std::filesystem::path m_ConfigFilepath;
		tur_unique<IConfigParser> m_ConfigParser;
		ConfigData m_Config;
	};
}