#pragma once
#include <nlohmann/json.hpp>

#include "Core/Engine/ApplicationSpecification.hpp"
#include "Core/Window/WindowProperties.hpp"
#include "Graphics/GraphicsSpecification.hpp"
#include "Graphics/Vulkan/VulkanArguments.hpp"

namespace tur
{
	struct ConfigData
	{
		ApplicationSpecification applicationSpecification;
		WindowProperties windowProperties;
		GraphicsSpecification graphicsSpecifications;
		vulkan::VulkanArguments vulkanArguments;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ConfigData, 
			applicationSpecification, 
			windowProperties, 
			graphicsSpecifications, 
			vulkanArguments
		);
	};
}