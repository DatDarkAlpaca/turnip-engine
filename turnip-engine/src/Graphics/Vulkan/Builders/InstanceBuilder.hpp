#pragma once
#include <optional>
#include <vulkan/vulkan.hpp>

#include "Common.h"
#include "Graphics/Vulkan/Objects/Objects.h"

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
	class VulkanInstanceBuilder
	{
		using VulkanDebugSeverityFlags = vk::Flags<vk::DebugUtilsMessageSeverityFlagBitsEXT>;
		using VulkanMessageTypeFlags = vk::Flags<vk::DebugUtilsMessageTypeFlagBitsEXT>;

		using VulkanDebugSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT;
		using VulkanMessageType = vk::DebugUtilsMessageTypeFlagBitsEXT;

		using VulkanDebugCallback = PFN_vkDebugUtilsMessengerCallbackEXT;

	public:
		VulkanInstanceBuilder();

	public:
		std::optional<Instance> Build();

	public:
		void DisplayVulkanAPIVersion() const;

	public:
		// Application Information:
		VulkanInstanceBuilder& SetAppName(const std::string& applicationName);
		VulkanInstanceBuilder& SetEngineName(const std::string& engineName);

		VulkanInstanceBuilder& SetApplicationVersion(uint32_t applicationVersion);
		VulkanInstanceBuilder& SetApplicationVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t variant = 0);
		VulkanInstanceBuilder& SetEngineVersion(uint32_t applicationVersion);
		VulkanInstanceBuilder& SetEngineVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t variant = 0);

		VulkanInstanceBuilder& SetAPIVersion(uint32_t applicationVersion);
		VulkanInstanceBuilder& SetAPIVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t variant = 0);

		// Layers & Extensions:
		VulkanInstanceBuilder& AddLayer(const char* layerName);
		VulkanInstanceBuilder& AddValidationLayer();
		VulkanInstanceBuilder& AddLayers(const std::vector<const char*>& layers);

		VulkanInstanceBuilder& AddExtension(const char* extensionName);
		VulkanInstanceBuilder& AddDebugUtilsExtension();
		VulkanInstanceBuilder& AddExtensions(const std::vector<const char*>& extensions);
		
		VulkanInstanceBuilder& ToggleKHRSurfaceFlag(bool state);
		VulkanInstanceBuilder& ToggleWindowingSurfaceFlag(bool state);
		VulkanInstanceBuilder& SetHeadless();
		VulkanInstanceBuilder& SetNotHeadless();

		// Debug Messenger:
		VulkanInstanceBuilder& UseDebugMessenger(bool value);

		VulkanInstanceBuilder& UseDefaultDebugCallback();
		VulkanInstanceBuilder& SetDebugCallback(VulkanDebugCallback callback);

		VulkanInstanceBuilder& SetDebugMessengerSeverity(VulkanDebugSeverity severity);
		VulkanInstanceBuilder& AddDebugMessengerSeverity(VulkanDebugSeverity severity);

		VulkanInstanceBuilder& SetDebugMessengerType(VulkanMessageType type);
		VulkanInstanceBuilder& AddDebugMessengerType(VulkanMessageType type);

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
			bool useDebugMessenger = true;
#else
			bool useDebugMessenger = false;
#endif
			bool addedValidationLayer = false, addDebugExtensions = false;

		} m_Information;
	};
}