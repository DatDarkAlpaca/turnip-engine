#pragma once
#include <nlohmann/json.hpp>
#include <filesystem>

#include "core/config/config_reader.hpp"
#include "core/config/config_writer.hpp"

#include "graphics/graphics_api.hpp"
#include "common.hpp"

namespace tur
{
	struct EngineSpecification
	{
		std::string applicationName = "Default Application";
		u32 major = 1, minor = 0, patch = 0, variant = 0;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(
			EngineSpecification,
			applicationName,
			major,
			minor,
			patch,
			variant
		);
	};

	struct WindowSpecification
	{
		enum Position : int { DEFAULT = -1 };

	public:
		std::string windowTitle = "TurnipEngine v1.0";
		glm::vec2 dimensions = glm::vec2(640, 480);
		glm::vec2 position = glm::vec2(Position::DEFAULT, Position::DEFAULT);
		glm::vec2 minSize = glm::vec2(0);
		glm::vec2 maxSize = dimensions;

	public:
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(WindowSpecification, windowTitle, dimensions, position, minSize, maxSize);
	};
}

namespace tur
{
	struct GraphicsConfig
	{
	public:
		GraphicsAPI api = GraphicsAPI::UNKNOWN;
		u8 major = 0;
		u8 minor = 0;
		u8 patch = 0;
		u8 variant = 0;
	};

	inline void to_json(nlohmann::json& json, const GraphicsConfig& specification)
	{
		json["api"] = get_graphics_api_name(specification.api);
		json["major"] = specification.major;
		json["minor"] = specification.minor;
		json["patch"] = specification.patch;
		json["variant"] = specification.variant;
	}

	inline void from_json(const nlohmann::json& json, GraphicsConfig& specification)
	{
		specification.api = get_graphics_api_from_string(json.at("api"));
		specification.major = json.at("major");
		specification.minor = json.at("minor");
		specification.patch = json.at("patch");
		specification.variant = json.at("variant");
	}
}

namespace tur
{
	struct ConfigData
	{
		EngineSpecification engineSpecification;
		WindowSpecification windowSpecification;
		GraphicsConfig graphicsConfig;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ConfigData, engineSpecification, windowSpecification, graphicsConfig);
	};

	inline void initialize_config_data(const std::filesystem::path& filepath)
	{
		if (!std::filesystem::exists(filepath))
		{
			std::ofstream file(filepath.string());

			ConfigWriter writer(filepath);
			ConfigData defaultData;
			writer.write(defaultData);
		}
	}
}