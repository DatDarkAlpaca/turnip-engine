#pragma once
#include <vulkan/vulkan.hpp>

namespace tur::vulkan
{
	struct Instance
	{
		vk::Instance instanceHandle;
		vk::DebugUtilsMessengerEXT debugMessenger;
		bool enablePresentation = false;
	};
}