#pragma once
#include <vulkan/vulkan.hpp>

namespace tur::vulkan
{
	struct Instance
	{
		vk::Instance instanceHandle;
		vk::DebugUtilsMessengerEXT debugMessenger;
		vk::DispatchLoaderDynamic DLDI;

		bool enablePresentation = false;
	};
}