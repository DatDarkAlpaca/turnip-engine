#include "pch.hpp"
#include "instance_builder.hpp"

#include "graphics/vulkan/vulkan_constants.hpp"
#include "graphics/vulkan/vulkan_helpers.hpp"
#include "platform/platform.hpp"

namespace tur::vulkan
{
	static const char* get_message_severity_name(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity)
	{
		switch (messageSeverity)
		{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				return "VERBOSE";

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				return "ERROR";

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				return "WARNING";

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				return "INFO";

			default:
				return "UNKNOWN";
		}
	}

	static const char* get_message_type_name(VkDebugUtilsMessageTypeFlagsEXT messageType)
	{
		if (messageType == 7)
			return "General | Validation | Performance";

		if (messageType == 6)
			return "Validation | Performance";

		if (messageType == 5)
			return "General | Performance";

		if (messageType == 4)
			return "Performance";

		if (messageType == 3)
			return "General | Validation";

		if (messageType == 2)
			return "Validation";

		if (messageType == 1)
			return "General";

		return "Unknown";
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL DefaultDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	)
	{
		auto severity = get_message_severity_name(messageSeverity);
		auto type = get_message_type_name(messageType);

		TUR_LOG_ERROR("[Vulkan: {}] [{}]: {}", severity, type, pCallbackData->pMessage);
		return VK_FALSE;
	}
}

namespace tur::vulkan
{
	void initialize_instance(VulkanState& state, const ConfigData& configuration)
	{
		auto& appConfig = configuration.applicationSpecification;
		auto& engineConfig = configuration.engineSpecification;
		auto& graphicsConfig = configuration.graphicsSpecification;

		auto& instanceConfig = configuration.vulkanConfiguration.instanceRequirements;
		
		state.apiVersion = VK_MAKE_API_VERSION(graphicsConfig.variant, graphicsConfig.major, graphicsConfig.minor, graphicsConfig.patch);
		state.requiresDrawing = instanceConfig.enableDrawing;
		state.validationEnabled = instanceConfig.enableValidationLayers;

#ifdef TUR_DEBUG
		state.validationEnabled = true;
#endif

		// Application information:
		vk::ApplicationInfo applicationInformation = vk::ApplicationInfo(
			appConfig.applicationName.c_str(),
			VK_MAKE_API_VERSION(appConfig.variant, appConfig.major, appConfig.minor, appConfig.patch),
			TUR_ENGINE_NAME,
			VK_MAKE_API_VERSION(engineConfig.variant, engineConfig.major, engineConfig.minor, engineConfig.patch),
			state.apiVersion
		);

		// Extensions & Layers:
		std::vector<const char*> layers;
		for (const auto& layer : instanceConfig.layers)
			layers.push_back(layer.c_str());

		std::vector<const char*> extensions; 
		for (const auto& extension : instanceConfig.extensions)
			layers.push_back(extension.c_str());

		if (state.validationEnabled)
		{
			layers.push_back(ValidationLayerName);
			extensions.push_back(DebugUtilsExtensionName);
		}

		std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();
		if (state.requiresDrawing)
		{
			extensions.push_back(SurfaceExtensionName);

			auto availableExtensionNames = tur::get_vulkan_extension_platform_surface_names();
			for (const char* extensionName : availableExtensionNames)
				extensions.push_back(extensionName);
		}

		auto validationResults = validate_layers(vk::enumerateInstanceLayerProperties(), layers);
		if (!validationResults.success)
		{
			TUR_LOG_ERROR("[Vulkan]: Unsupported instance layers:");
			for (const auto& layer : validationResults.unsupported)
				TUR_LOG_ERROR(" * {}", layer);

			return;
		}

		validationResults = validate_extensions(supportedExtensions, extensions);
		if (!validationResults.success)
		{
			TUR_LOG_ERROR("[Vulkan]: Unsupported instance extensions:");
			for (const auto& extension : validationResults.unsupported)
				TUR_LOG_ERROR(" * {}", extension);

			return;
		}

		// Instance:
		vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(
			vk::InstanceCreateFlags(),
			&applicationInformation,
			static_cast<uint32_t>(layers.size()), layers.data(),
			static_cast<uint32_t>(extensions.size()), extensions.data()
		);

		try
		{
			state.instance = vk::createInstance(createInfo);
		}
		catch (const vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to create vulkan instance: {}", err.what());
			return;
		}

		// Messenger Creation:
		if (!state.validationEnabled)
			return;

		vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT(
			vk::DebugUtilsMessengerCreateFlagsEXT(),
			DefaultDebugMessageSeverity,
			DefaultDebugMessageType,
			DefaultDebugCallback,
			nullptr
		);

		vk::DispatchLoaderDynamic DLDI(state.instance, vkGetInstanceProcAddr);
		state.debugMessenger = state.instance.createDebugUtilsMessengerEXT(debugCreateInfo, nullptr, DLDI);
		state.DLDI = DLDI;
	}
}