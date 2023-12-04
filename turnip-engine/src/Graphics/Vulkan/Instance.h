#pragma once
#include <optional>
#include <vulkan/vulkan.hpp>

#include "Constants.h"
#include "Core/Defines.h"
#include "Core/Logger/Logger.h"

// Helper functions:
namespace
{
	inline bool CheckExtensionSupport(const std::vector<vk::ExtensionProperties>& availableExtensions, const char* extensionName)
	{
		if (!extensionName)
			return false;

		for (const auto& extensionProperties : availableExtensions)
		{
			if (strcmp(extensionName, extensionProperties.extensionName) == 0)
				return true;
		}

		return false;
	}

	inline bool CheckLayerSupported(const std::vector<vk::LayerProperties>& availableLayers, const char* layerName)
	{
		if (!layerName) 
			return false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
				return true;
		}

		return false;
	}

	inline bool ValidateExtensions(const std::vector<const char*>& extensions)
	{
		std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

		bool found;
		for (const char* extension : extensions)
		{
			found = CheckExtensionSupport(supportedExtensions, extension);
			if (!found)
			{
				TUR_LOG_ERROR("Extension {} is not supported.", extension);
				return false;
			}
		}

		return true;
	}

	inline bool ValidateLayers(const std::vector<const char*>& layers)
	{
		std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

		bool found;
		for (const char* layer : layers)
		{
			found = CheckLayerSupported(supportedLayers, layer);
			if (!found)
			{
				TUR_LOG_ERROR("Layer {} is not supported.", layer);
				return false;
			}
		}

		return true;
	}
}

// Debug Messenger Helpers:
namespace tur::vulkan
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

	inline VKAPI_ATTR VkBool32 VKAPI_CALL DefaultDebugCallback (
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	)
	{
		auto severity = GetMessageSeverityName(messageSeverity);
		auto type = GetMessageTypeName(messageType);

		TUR_LOG_DEBUG("[Vulkan: {}] [{}]: {}", severity, type, pCallbackData->pMessage);
		return VK_FALSE;
	}
}

namespace tur::vulkan
{
	struct InstanceOutput
	{
		vk::Instance instanceHandle;
		vk::DebugUtilsMessengerEXT debugMessenger;
	};

	class VulkanInstanceBuilder
	{
		using VulkanDebugSeverityFlags = vk::Flags<vk::DebugUtilsMessageSeverityFlagBitsEXT>;
		using VulkanMessageTypeFlags = vk::Flags<vk::DebugUtilsMessageTypeFlagBitsEXT>;

		using VulkanDebugSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT;
		using VulkanMessageType = vk::DebugUtilsMessageTypeFlagBitsEXT;

		using VulkanDebugCallback = PFN_vkDebugUtilsMessengerCallbackEXT;

	public:
		explicit VulkanInstanceBuilder()
		{
#ifdef TUR_DEBUG
			AddValidationLayer();
			AddDebugUtilsExtension();
#endif
		}

	public:
		std::optional<InstanceOutput> Build()
		{
			InstanceOutput output;

			// Application information:
			vk::ApplicationInfo applicationInformation = vk::ApplicationInfo(
				m_Information.applicationName.c_str(),
				m_Information.applicationVersion,
				m_Information.engineName.c_str(),
				m_Information.engineVersion,
				m_Information.apiVersion
			);

			// Extensions & Layers:
			std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

			if (!ValidateExtensions(m_Information.extensions))
			{
				TUR_LOG_ERROR("[Vulkan Instance]: One or more requested extensions is not supported.");
				return std::nullopt;
			}

			if (!ValidateLayers(m_Information.layers))
			{
				TUR_LOG_ERROR("[Vulkan Instance]: One or more requested layers is not supported.");
				return std::nullopt;
			}

			RequestSurfaceExtension(supportedExtensions);
	
			RequestWindowingExtensions(supportedExtensions);

			// Instance:
			vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(
				vk::InstanceCreateFlags(),
				&applicationInformation,
				static_cast<uint32_t>(m_Information.layers.size()), m_Information.layers.data(),
				static_cast<uint32_t>(m_Information.extensions.size()), m_Information.extensions.data()
			);

			try
			{
				output.instanceHandle = vk::createInstance(createInfo);
			}
			catch (vk::SystemError err)
			{
				TUR_LOG_ERROR("Failed to create vulkan instance: {}", err.what());
				return std::nullopt;
			}

			// Messenger Creation:
			if (!m_Information.useDebugMessenger)
				return output;

			vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT(
				vk::DebugUtilsMessengerCreateFlagsEXT(),
				m_Information.messageSeverity,
				m_Information.messageType,
				m_Information.debugCallback,
				nullptr
			);

			vk::DispatchLoaderDynamic DLDI(output.instanceHandle, vkGetInstanceProcAddr);
			output.debugMessenger = output.instanceHandle.createDebugUtilsMessengerEXT(debugCreateInfo, nullptr, DLDI);

			return output;
		}

	public:
		inline void DisplayVulkanAPIVersion() const
		{
			auto variant = VK_API_VERSION_VARIANT(m_Information.apiVersion);
			auto major = VK_API_VERSION_MAJOR(m_Information.apiVersion);
			auto minor = VK_API_VERSION_MINOR(m_Information.apiVersion);
			auto patch = VK_API_VERSION_PATCH(m_Information.apiVersion);

			TUR_LOG_DEBUG("Vulkan Version: {}.{}.{} - {}", major, minor, patch, variant);
		}

	public:
		// Application Information:
		VulkanInstanceBuilder& SetAppName(const std::string& applicationName)
		{
			m_Information.applicationName = applicationName;
			return *this;
		}
		VulkanInstanceBuilder& SetEngineName(const std::string& engineName)
		{
			m_Information.engineName = engineName;
			return *this;
		}

		VulkanInstanceBuilder& SetApplicationVersion(uint32_t applicationVersion)
		{
			m_Information.applicationVersion = applicationVersion;
			return *this;
		}
		VulkanInstanceBuilder& SetApplicationVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t variant = 0)
		{
			m_Information.applicationVersion = VK_MAKE_API_VERSION(variant, major, minor, patch);
			return *this;
		}

		VulkanInstanceBuilder& SetEngineVersion(uint32_t applicationVersion)
		{
			m_Information.engineVersion = applicationVersion;
			return *this;
		}
		VulkanInstanceBuilder& SetEngineVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t variant = 0)
		{
			m_Information.engineVersion = VK_MAKE_API_VERSION(variant, major, minor, patch);
			return *this;
		}

		VulkanInstanceBuilder& SetAPIVersion(uint32_t applicationVersion)
		{
			m_Information.apiVersion = applicationVersion;
			return *this;
		}
		VulkanInstanceBuilder& SetAPIVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t variant = 0)
		{
			m_Information.apiVersion = VK_MAKE_API_VERSION(variant, major, minor, patch);
			return *this;
		}

		// Layers & Extensions:
		VulkanInstanceBuilder& AddLayer(const char* layerName)
		{
			m_Information.layers.push_back(layerName);
			return *this;
		}
		VulkanInstanceBuilder& AddValidationLayer()
		{
			m_Information.layers.push_back(ValidationLayerName);
			return *this;
		}
		VulkanInstanceBuilder& AddLayers(const std::vector<const char*>& layers)
		{
			for(const auto& layer : layers)
				m_Information.layers.push_back(layer);

			return *this;
		}

		VulkanInstanceBuilder& AddExtension(const char* extensionName)
		{
			m_Information.extensions.push_back(extensionName);
			return *this;
		}
		VulkanInstanceBuilder& AddDebugUtilsExtension()
		{
			m_Information.extensions.push_back(DebugUtilsExtensionName);
			return *this; 
		}
		VulkanInstanceBuilder& AddExtensions(const std::vector<const char*>& extensions)
		{
			for (const auto& extension : extensions)
				m_Information.extensions.push_back(extension);

			return *this;
		}
		
		VulkanInstanceBuilder& ToggleKHRSurfaceFlag(bool state)
		{
			m_Information.disableKHRSurface = state;
			return *this;
		}
		VulkanInstanceBuilder& ToggleWindowingSurfaceFlag(bool state)
		{
			m_Information.disableWindowingSurface = state;
			return *this;
		}
		VulkanInstanceBuilder& SetHeadless()
		{
			m_Information.disableKHRSurface = true;
			m_Information.disableWindowingSurface = true;
			return *this;
		}
		VulkanInstanceBuilder& SetNotHeadless()
		{
			m_Information.disableKHRSurface = false;
			m_Information.disableWindowingSurface = false;
			return *this;
		}

		// Debug Messenger:
		VulkanInstanceBuilder& UseDebugMessenger(bool value)
		{
			if (value)
			{
				AddValidationLayer();
				AddDebugUtilsExtension();
			}

			m_Information.useDebugMessenger = value;
			return *this;
		}

		VulkanInstanceBuilder& UseDefaultDebugCallback()
		{
			AddValidationLayer();
			AddDebugUtilsExtension();

			m_Information.useDebugMessenger = true;
			m_Information.debugCallback = DefaultDebugCallback;
			return *this;
		}
		VulkanInstanceBuilder& SetDebugCallback(VulkanDebugCallback callback)
		{
			AddValidationLayer();
			AddDebugUtilsExtension();

			m_Information.useDebugMessenger = true;
			m_Information.debugCallback = callback;
			return *this;
		}

		VulkanInstanceBuilder& SetDebugMessengerSeverity(VulkanDebugSeverity severity)
		{
			m_Information.messageSeverity = severity;
			return *this;
		}
		VulkanInstanceBuilder& AddDebugMessengerSeverity(VulkanDebugSeverity severity)
		{
			m_Information.messageSeverity |= severity;
			return *this;
		}

		VulkanInstanceBuilder& SetDebugMessengerType(VulkanMessageType type)
		{
			m_Information.messageType = type;
			return *this;
		}
		VulkanInstanceBuilder& AddDebugMessengerType(VulkanMessageType type)
		{
			m_Information.messageType = m_Information.messageType | type;
			return *this;
		}

	private:
		void RequestSurfaceExtension(const std::vector<vk::ExtensionProperties>& supportedExtensions)
		{
			if (m_Information.disableKHRSurface)
				return;

			bool supportsKHRSurface = CheckExtensionSupport(supportedExtensions, SurfaceExtensionName);
			if (supportsKHRSurface)
				m_Information.extensions.push_back(SurfaceExtensionName);
			else
				TUR_LOG_CRITICAL("This device does not support the KHR Surface extension");
		}

		void RequestWindowingExtensions(const std::vector<vk::ExtensionProperties>& supportedExtensions)
		{
			if (m_Information.disableWindowingSurface)
				return;

#ifdef TUR_PLATFORM_WIN32
			if (CheckExtensionSupport(supportedExtensions, WIN32_SurfaceExtensionName))
				m_Information.extensions.push_back(WIN32_SurfaceExtensionName);
			else
				TUR_LOG_CRITICAL("This device does not support the WIN32 Surface extension");

#elif defined(TUR_PLATFORM_LINUX)
			bool supportsLinuxSurface = CheckExtensionSupport(supportedExtensions, XCB_SurfaceExtensionName);
			if (supportsLinuxSurface)
				m_Information.extensions.push_back(XCB_SurfaceExtensionName);

			supportsLinuxSurface = CheckExtensionSupport(supportedExtensions, XLIB_SurfaceExtensionName);
			if (supportsLinuxSurface)
				m_Information.extensions.push_back(XLIB_SurfaceExtensionName);

			supportsLinuxSurface = CheckExtensionSupport(supportedExtensions, WAYLAND_SurfaceExtensionName);
			if (supportsLinuxSurface)
				m_Information.extensions.push_back(WAYLAND_SurfaceExtensionName);

			if (!supportsLinuxSurface)
				TUR_LOG_CRITICAL("This linux device does not support any of the following supported surface extensions: XCB, XLIB, Wayland");

#elif defined(TUR_PLATFORM_APPLE)
			if (CheckExtensionSupport(supportedExtensions, METAL_SurfaceExtensionName))
				m_Information.extensions.push_back(METAL_SurfaceExtensionName);
			else
				TUR_LOG_CRITICAL("This device does not support the Apple/Metal Surface extension");

#elif defined(TUR_PLATFORM_ANDROID)
			if (CheckExtensionSupport(supportedExtensions, ANDROID_SurfaceExtensionName))
				m_Information.extensions.push_back(ANDROID_SurfaceExtensionName);
			else
				TUR_LOG_CRITICAL("This device does not support the Android Surface extension");
#endif
		}

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
		} m_Information;
	};
}