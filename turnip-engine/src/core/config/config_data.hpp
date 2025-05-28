#pragma once
#include <vulkan/vulkan.hpp>
#include <filesystem>

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

#include "utils/json/json_file.hpp"

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

			ConfigData defaultData;
			json_write_file(filepath, defaultData);
		}
	}
}