#pragma once
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
	};
}