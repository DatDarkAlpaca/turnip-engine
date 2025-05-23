#pragma once
#include <nlohmann/json.hpp>
#include <vulkan/vulkan.hpp>
#include <filesystem>

#include "utils/json/json_reader.hpp"
#include "utils/json/json_writer.hpp"

#include "graphics/graphics_api.hpp"
#include "common.hpp"

#include "application_configuration.hpp"
#include "engine_configuration.hpp"
#include "window_configuration.hpp"
#include "graphics_configuration.hpp"
#include "vulkan_configuration.hpp"
#include "quad_renderer_configuration.hpp"
#include "instanced_quad_renderer_configuration.hpp"
#include "scripting_data.hpp"

namespace tur
{
	struct ConfigData
	{
		EngineSpecification engineSpecification;
		ApplicationSpecification applicationSpecification;
		WindowProperties windowProperties;
		GraphicsSpecification graphicsSpecification;
		QuadRendererInformation quadRendererInformation;
		InstancedQuadRendererInformation instancedQuadRendererInformation;
		vulkan::VulkanConfiguration vulkanConfiguration;
		ScriptingInfo scriptingInfo;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ConfigData, 
			engineSpecification, 
			applicationSpecification, 
			windowProperties,
			graphicsSpecification,
			quadRendererInformation,
			instancedQuadRendererInformation,
			vulkanConfiguration,
			scriptingInfo
		);
	};

	inline void initialize_config_data(const std::filesystem::path& filepath)
	{
		if (!std::filesystem::exists(filepath))
		{
			std::ofstream file(filepath.string());

			JsonWriter writer(filepath);
			ConfigData defaultData;
			writer.write(defaultData);
		}
	}
}