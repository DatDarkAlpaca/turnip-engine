#pragma once
#include <vulkan/vulkan.hpp>

namespace tur::vulkan
{
	struct InstanceObject
	{
		vk::Instance instance;
		vk::DebugUtilsMessengerEXT debugMessenger;
		vk::DispatchLoaderDynamic DLDI;
	};
}