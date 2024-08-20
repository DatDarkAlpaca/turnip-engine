#pragma once
#include "ConfigParser.hpp"
#include "ConfigData.hpp"
#include "Util/Json/JsonFile.hpp"

namespace tur
{
	class JSONConfigParser : public IConfigParser
	{
	public:
		ConfigData Load(const std::filesystem::path& filepath) override
		{
			ConfigData data;

			nlohmann::json jsonObject = ReadJson(filepath);

			data.applicationSpecification = ParseApplicationSpecification(jsonObject);
			data.windowProperties = ParseWindowProperties(jsonObject);
			data.graphicsSpecifications = ParseGraphicsSpecifications(jsonObject);
			data.vulkanArguments = ParseVulkanArguments(jsonObject);

			return data;
		}

		void Save(const ConfigData& config, const std::filesystem::path& filepath)
		{
			nlohmann::ordered_json jsonObject;

			const auto& appProp		 = config.applicationSpecification;
			const auto& windowProp	 = config.windowProperties;
			const auto& graphicsProp = config.graphicsSpecifications;
			const auto& vulkanProp   = config.vulkanArguments;

			// Application:
			{
				jsonObject["Application"]["name"] = appProp.applicationName;
				jsonObject["Application"]["versionMajor"] = appProp.versionMajor;
				jsonObject["Application"]["versionMinor"] = appProp.versionMinor;
				jsonObject["Application"]["versionPatch"] = appProp.versionPatch;
				jsonObject["Application"]["versionVariant"] = appProp.versionVariant;
			}
			
			// Window:
			{
				jsonObject["Window"]["width"] = windowProp.dimensions.x;
				jsonObject["Window"]["height"] = windowProp.dimensions.y;
				jsonObject["Window"]["title"] = windowProp.windowTitle;

				jsonObject["Window"]["x"] = windowProp.position.x;
				if (windowProp.position.x == WindowProperties::Position::DEFAULT)
					jsonObject["Window"]["x"] = "DEFAULT";

				jsonObject["Window"]["y"] = windowProp.position.y;
				if (windowProp.position.y == WindowProperties::Position::DEFAULT)
					jsonObject["Window"]["y"] = "DEFAULT";

				jsonObject["Window"]["minWidth"] = windowProp.minSize.x;
				jsonObject["Window"]["minHeight"] = windowProp.minSize.y;
				jsonObject["Window"]["maxWidth"] = windowProp.maxSize.x;
				jsonObject["Window"]["maxHeight"] = windowProp.maxSize.y;
			}
			
			// Graphics:
			{
				jsonObject["Graphics"]["API"] = GetGraphicsAPIString(graphicsProp.api);
				jsonObject["Graphics"]["versionMajor"] = graphicsProp.major;
				jsonObject["Graphics"]["versionMinor"] = graphicsProp.minor;
				jsonObject["Graphics"]["versionPatch"] = graphicsProp.patch;
				jsonObject["Graphics"]["versionVariant"] = graphicsProp.variant;
			}
			
			// Vulkan:
			{
				jsonObject["Vulkan"]["layers"] = vulkanProp.layers;
				jsonObject["Vulkan"]["extensions"] = vulkanProp.extensions;
				jsonObject["Vulkan"]["enablePresentation"] = vulkanProp.enablePresentation;
				jsonObject["Vulkan"]["addValidationLayer"] = vulkanProp.addValidationLayer;
				jsonObject["Vulkan"]["addDebugExtensions"] = vulkanProp.addDebugExtensions;
				jsonObject["Vulkan"]["useDebugMessenger"] = vulkanProp.useDebugMessenger;
			}
			
			std::ofstream jsonFile(filepath);
			jsonFile << std::setw(4) << jsonObject << std::endl;
		}

	private:
		ApplicationSpecification ParseApplicationSpecification(const nlohmann::json& jsonObject)
		{
			ApplicationSpecification appSpecs;

			appSpecs.applicationName = jsonObject["Application"]["name"];
			appSpecs.versionMajor = jsonObject["Application"]["versionMajor"];
			appSpecs.versionMinor = jsonObject["Application"]["versionMinor"];
			appSpecs.versionPatch = jsonObject["Application"]["versionPatch"];
			appSpecs.versionVariant = jsonObject["Application"]["versionVariant"];

			return appSpecs;
		}

		WindowProperties ParseWindowProperties(const nlohmann::json& jsonObject)
		{
			WindowProperties windowProperties;

			windowProperties.dimensions.x = jsonObject["Window"]["width"];
			windowProperties.dimensions.y = jsonObject["Window"]["height"];
			windowProperties.windowTitle  = jsonObject["Window"]["title"];

			if (jsonObject["Window"]["x"] != "DEFAULT")
				windowProperties.position.x = jsonObject["Window"]["x"];

			if (jsonObject["Window"]["y"] != "DEFAULT")
				windowProperties.position.y = jsonObject["Window"]["y"];

			windowProperties.minSize.x = jsonObject["Window"]["minWidth"];
			windowProperties.minSize.y = jsonObject["Window"]["minHeight"];

			windowProperties.maxSize.x = jsonObject["Window"]["maxWidth"];
			windowProperties.maxSize.y = jsonObject["Window"]["maxHeight"];

			return windowProperties;
		}

		GraphicsSpecification ParseGraphicsSpecifications(const nlohmann::json& jsonObject)
		{
			GraphicsSpecification graphicsSpecification;

			graphicsSpecification.api = GetGraphicsAPIFromString(jsonObject["Graphics"]["API"]);

			graphicsSpecification.major   = jsonObject["Graphics"]["versionMajor"];
			graphicsSpecification.minor   = jsonObject["Graphics"]["versionMinor"];
			graphicsSpecification.patch   = jsonObject["Graphics"]["versionPatch"];
			graphicsSpecification.variant = jsonObject["Graphics"]["versionVariant"];

			return graphicsSpecification;
		}

		vulkan::VulkanArguments ParseVulkanArguments(const nlohmann::json& jsonObject)
		{
			vulkan::VulkanArguments arguments;

			arguments.layers     = jsonObject["Vulkan"]["layers"].get<std::vector<std::string>>();
			arguments.extensions = jsonObject["Vulkan"]["extensions"].get<std::vector<std::string>>();

			arguments.enablePresentation = jsonObject["Vulkan"]["enablePresentation"];
			arguments.addValidationLayer = jsonObject["Vulkan"]["addValidationLayer"];
			arguments.addDebugExtensions = jsonObject["Vulkan"]["addDebugExtensions"];
			arguments.useDebugMessenger  = jsonObject["Vulkan"]["useDebugMessenger"];

			return arguments;
		}
	};
}