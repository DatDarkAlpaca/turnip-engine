#pragma once
#include <optional>
#include <vulkan/vulkan.hpp>

#include "Graphics/Vulkan/Objects/Instance.hpp"
#include "Graphics/Vulkan/Constants.hpp"
#include "Common.hpp"

namespace
{
	inline const char* GetMessageSeverityName(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity)
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

	inline const char* GetMessageTypeName(VkDebugUtilsMessageTypeFlagsEXT messageType)
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

	inline VKAPI_ATTR VkBool32 VKAPI_CALL DefaultDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	)
	{
		auto severity = GetMessageSeverityName(messageSeverity);
		auto type = GetMessageTypeName(messageType);

		TUR_LOG_ERROR("[Vulkan: {}] [{}]: {}", severity, type, pCallbackData->pMessage);
		return VK_FALSE;
	}
}

namespace tur::vulkan
{
	class InstanceBuilder
	{
		using VulkanDebugSeverityFlags = vk::Flags<vk::DebugUtilsMessageSeverityFlagBitsEXT>;
		using VulkanMessageTypeFlags = vk::Flags<vk::DebugUtilsMessageTypeFlagBitsEXT>;

		using VulkanDebugSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT;
		using VulkanMessageType = vk::DebugUtilsMessageTypeFlagBitsEXT;

		using VulkanDebugCallback = PFN_vkDebugUtilsMessengerCallbackEXT;

	public:
		InstanceBuilder();

	public:
		std::optional<InstanceObject> Build();

	public:
		void DisplayVulkanAPIVersion() const;

	public:
		// Application Information:
		InstanceBuilder& SetAppName(const std::string& applicationName);
		InstanceBuilder& SetEngineName(const std::string& engineName);

		InstanceBuilder& SetApplicationVersion(uint32_t applicationVersion);
		InstanceBuilder& SetApplicationVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t variant = 0);
		InstanceBuilder& SetEngineVersion(uint32_t applicationVersion);
		InstanceBuilder& SetEngineVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t variant = 0);

		InstanceBuilder& SetAPIVersion(uint32_t applicationVersion);
		InstanceBuilder& SetAPIVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t variant = 0);

		// Layers & Extensions:
		InstanceBuilder& AddLayer(const char* layerName);
		InstanceBuilder& AddValidationLayer();
		InstanceBuilder& AddLayers(const std::vector<std::string>& layers);

		InstanceBuilder& AddExtension(const char* extensionName);
		InstanceBuilder& AddDebugUtilsExtension();
		InstanceBuilder& AddExtensions(const std::vector<std::string>& extensions);
		
		InstanceBuilder& ToggleKHRSurfaceFlag(bool state);
		InstanceBuilder& ToggleWindowingSurfaceFlag(bool state);
		InstanceBuilder& SetHeadless();
		InstanceBuilder& SetNotHeadless();

		// Debug Messenger:
		InstanceBuilder& UseDebugMessenger(bool value);

		InstanceBuilder& UseDefaultDebugCallback();
		InstanceBuilder& SetDebugCallback(VulkanDebugCallback callback);

		InstanceBuilder& SetDebugMessengerSeverity(VulkanDebugSeverity severity);
		InstanceBuilder& AddDebugMessengerSeverity(VulkanDebugSeverity severity);

		InstanceBuilder& SetDebugMessengerType(VulkanMessageType type);
		InstanceBuilder& AddDebugMessengerType(VulkanMessageType type);

	private:
		void RequestSurfaceExtension(const std::vector<vk::ExtensionProperties>& supportedExtensions);

		void RequestWindowingExtensions(const std::vector<vk::ExtensionProperties>& supportedExtensions);

	private:
		struct InstanceInformation
		{
			std::vector<const char*> layers, extensions;
			std::string engineName = "TurnipEngine";
			std::string applicationName;

			uint32_t applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
			uint32_t engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
			uint32_t apiVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);

			// Debug Messenger:
			VulkanDebugSeverityFlags messageSeverity = DefaultDebugMessageSeverity;
			VulkanMessageTypeFlags messageType = DefaultDebugMessageType;
			VulkanDebugCallback debugCallback = DefaultDebugCallback;

			bool disableKHRSurface = false;
			bool disableWindowingSurface = false;

#ifdef TUR_DEBUG
			bool addValidationLayer = true, addDebugExtensions = true, useDebugMessenger = true;
#else
			bool addValidationLayer = false, addDebugExtensions = false, useDebugMessenger = false;
#endif
		} m_Information;
	};
}