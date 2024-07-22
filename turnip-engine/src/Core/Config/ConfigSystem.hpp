#pragma once
#include <filesystem>
#include "Util/Json/JsonFile.hpp"

#include "Core/Engine/ApplicationSpecification.hpp"
#include "Core/Window/WindowProperties.hpp"
#include "Graphics/GraphicsSpecification.hpp"
#include "Graphics/Vulkan/VulkanArguments.hpp"

namespace tur
{
	class ConfigSystem
	{
	public:
		explicit ConfigSystem(const std::filesystem::path& configFilepath)
		{
			if (!std::filesystem::exists(configFilepath))
				CreateDefaultConfigFile(configFilepath);

			m_Config = ReadJson(configFilepath);
		}

		ConfigSystem() = default;

	public:
		ApplicationSpecification GetApplicationSpecification() const
		{
			ApplicationSpecification appSpecs;

			appSpecs.applicationName = m_Config["Application"]["name"];
			appSpecs.versionMajor = m_Config["Application"]["versionMajor"];
			appSpecs.versionMinor = m_Config["Application"]["versionMinor"];
			appSpecs.versionPatch = m_Config["Application"]["versionPatch"];
			appSpecs.versionVariant = m_Config["Application"]["versionVariant"];

			return appSpecs;
		}

		WindowProperties GetWindowProperties() const
		{
			WindowProperties windowProperties;

			windowProperties.dimensions.x = m_Config["Window"]["width"];
			windowProperties.dimensions.y = m_Config["Window"]["height"];
			windowProperties.windowTitle = m_Config["Window"]["title"];

			if (m_Config["Window"]["x"] != "DEFAULT")
				windowProperties.position.x = m_Config["Window"]["x"];

			if (m_Config["Window"]["y"] != "DEFAULT")
				windowProperties.position.y = m_Config["Window"]["y"];

			windowProperties.minSize.x = m_Config["Window"]["minWidth"];
			windowProperties.minSize.y = m_Config["Window"]["minHeight"];

			windowProperties.maxSize.x = m_Config["Window"]["maxWidth"];
			windowProperties.maxSize.y = m_Config["Window"]["maxHeight"];

			return windowProperties;
		}

		GraphicsSpecification GetGraphicsSpecification() const
		{
			GraphicsSpecification graphicsSpecification;

			auto graphicsAPI = m_Config["Graphics"]["API"];
			if (graphicsAPI == "OPENGL")
				graphicsSpecification.api = GraphicsAPI::OPENGL;

			if (graphicsAPI == "VULKAN")
				graphicsSpecification.api = GraphicsAPI::VULKAN;

			graphicsSpecification.major   = m_Config["Graphics"]["versionMajor"];
			graphicsSpecification.minor   = m_Config["Graphics"]["versionMinor"];
			graphicsSpecification.patch   = m_Config["Graphics"]["versionPatch"];
			graphicsSpecification.variant = m_Config["Graphics"]["versionVariant"];

			graphicsSpecification.options.usingBottomLeftOrigin
				= m_Config["Graphics"]["usingBottomLeftOrigin"];

			return graphicsSpecification;
		}

	public:
		vulkan::VulkanArguments GetVulkanArguments() const
		{
			vulkan::VulkanArguments arguments;

			arguments.layers = m_Config["Vulkan"]["layers"].get<std::vector<std::string>>();
			arguments.extensions = m_Config["Vulkan"]["extensions"].get<std::vector<std::string>>();

			arguments.enablePresentation = m_Config["Vulkan"]["enablePresentation"];
			arguments.addValidationLayer = m_Config["Vulkan"]["addValidationLayer"];
			arguments.addDebugExtensions = m_Config["Vulkan"]["addDebugExtensions"];
			arguments.useDebugMessenger  = m_Config["Vulkan"]["useDebugMessenger"];

			return arguments;
		}

	public:
		static void CreateDefaultConfigFile(const std::filesystem::path& filepath)
		{
			nlohmann::ordered_json jsonObject;

			jsonObject["Application"]["name"]			= "Default Application";
			jsonObject["Application"]["versionMajor"]	= 1;
			jsonObject["Application"]["versionMinor"]	= 0;
			jsonObject["Application"]["versionPatch"]	= 0;
			jsonObject["Application"]["versionVariant"] = 0;

			jsonObject["Window"]["width"]		= 800;
			jsonObject["Window"]["height"]		= 600;
			jsonObject["Window"]["title"]		= "TurnipEngine v1.0";
			jsonObject["Window"]["x"]			= "DEFAULT";
			jsonObject["Window"]["y"]			= "DEFAULT";
			jsonObject["Window"]["minWidth"]	= 100;
			jsonObject["Window"]["minHeight"]	= 1000;
			jsonObject["Window"]["maxWidth"]	= 10000;
			jsonObject["Window"]["maxHeight"]	= 10000;

			jsonObject["Graphics"]["API"]					= "OPENGL";
			jsonObject["Graphics"]["versionMajor"]			= 4;
			jsonObject["Graphics"]["versionMinor"]			= 5;
			jsonObject["Graphics"]["versionPatch"]			= 0;
			jsonObject["Graphics"]["versionVariant"]		= 0;
			jsonObject["Graphics"]["usingBottomLeftOrigin"]	= true;

			jsonObject["Vulkan"]["layers"]				 = std::vector<const char*> {};
			jsonObject["Vulkan"]["extensions"]			 = std::vector<const char*> {};
			jsonObject["Vulkan"]["enablePresentation"]	 = true;
			jsonObject["Vulkan"]["addValidationLayer"]	 = false;
			jsonObject["Vulkan"]["addDebugExtensions"]	 = false;
			jsonObject["Vulkan"]["useDebugMessenger"]    = false;

			std::ofstream jsonFile(filepath);
			jsonFile << std::setw(4) << jsonObject << std::endl;
		}

	private:
		nlohmann::json m_Config;
	};
}