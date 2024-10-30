#pragma once
#include <vulkan/vulkan.hpp>
#include "Util/File.h"

namespace tur::vulkan
{
	class Shader
	{
	public:
		vk::ShaderModule shaderModule;
	};
}