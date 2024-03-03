#include "pch.h"
#include "InstanceBuilder.h"

static bool CheckExtensionSupport(const std::vector<vk::ExtensionProperties>& availableExtensions, const char* extensionName)
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

static bool CheckLayerSupported(const std::vector<vk::LayerProperties>& availableLayers, const char* layerName)
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

static bool ValidateExtensions(const std::vector<const char*>& extensions)
{
	std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

	for (const char* extension : extensions)
	{
		if (!CheckExtensionSupport(supportedExtensions, extension))
		{
			TUR_LOG_ERROR("Extension {} is not supported.", extension);
			return false;
		}
	}

	return true;
}

static bool ValidateLayers(const std::vector<const char*>& layers)
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

namespace tur::vulkan
{
	VulkanInstanceBuilder::VulkanInstanceBuilder()
	{
#ifdef TUR_DEBUG
		AddValidationLayer();
		AddDebugUtilsExtension();
		UseDefaultDebugCallback();
#endif
	}

	std::optional<Instance> VulkanInstanceBuilder::Build()
	{
		Instance output;

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
		catch (const vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to create vulkan instance: {}", err.what());
			return std::nullopt;
		}

		// Messenger Creation:
		output.enablePresentation = !m_Information.disableKHRSurface && !m_Information.disableWindowingSurface;

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

	void VulkanInstanceBuilder::DisplayVulkanAPIVersion() const
	{
		auto variant = VK_API_VERSION_VARIANT(m_Information.apiVersion);
		auto major = VK_API_VERSION_MAJOR(m_Information.apiVersion);
		auto minor = VK_API_VERSION_MINOR(m_Information.apiVersion);
		auto patch = VK_API_VERSION_PATCH(m_Information.apiVersion);

		TUR_LOG_DEBUG("Vulkan Version: {}.{}.{} - {}", major, minor, patch, variant);
	}

	// Application Information:
	VulkanInstanceBuilder& VulkanInstanceBuilder::SetAppName(const std::string& applicationName)
	{
		m_Information.applicationName = applicationName;
		return *this;
	}
	
	VulkanInstanceBuilder& VulkanInstanceBuilder::SetEngineName(const std::string& engineName)
	{
		m_Information.engineName = engineName;
		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::SetApplicationVersion(uint32_t applicationVersion)
	{
		m_Information.applicationVersion = applicationVersion;
		return *this;
	}
	
	VulkanInstanceBuilder& VulkanInstanceBuilder::SetApplicationVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t variant)
	{
		m_Information.applicationVersion = VK_MAKE_API_VERSION(variant, major, minor, patch);
		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::SetEngineVersion(uint32_t applicationVersion)
	{
		m_Information.engineVersion = applicationVersion;
		return *this;
	}
	
	VulkanInstanceBuilder& VulkanInstanceBuilder::SetEngineVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t variant)
	{
		m_Information.engineVersion = VK_MAKE_API_VERSION(variant, major, minor, patch);
		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::SetAPIVersion(uint32_t applicationVersion)
	{
		m_Information.apiVersion = applicationVersion;
		return *this;
	}
	
	VulkanInstanceBuilder& VulkanInstanceBuilder::SetAPIVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t variant)
	{
		m_Information.apiVersion = VK_MAKE_API_VERSION(variant, major, minor, patch);
		return *this;
	}

	// Layers & Extensions:
	VulkanInstanceBuilder& VulkanInstanceBuilder::AddLayer(const char* layerName)
	{
		m_Information.layers.push_back(layerName);
		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::AddValidationLayer()
	{
		if (m_Information.addedValidationLayer)
			return *this;

		m_Information.layers.push_back(ValidationLayerName);
		m_Information.addedValidationLayer = true;
		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::AddLayers(const std::vector<const char*>& layers)
	{
		for (const auto& layer : layers)
			m_Information.layers.push_back(layer);

		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::AddExtension(const char* extensionName)
	{
		m_Information.extensions.push_back(extensionName);
		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::AddDebugUtilsExtension()
	{
		if (m_Information.addDebugExtensions)
			return *this;

		m_Information.extensions.push_back(DebugUtilsExtensionName);
		m_Information.addDebugExtensions = true;
		return *this;
	}
	
	VulkanInstanceBuilder& VulkanInstanceBuilder::AddExtensions(const std::vector<const char*>& extensions)
	{
		for (const auto& extension : extensions)
			m_Information.extensions.push_back(extension);

		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::ToggleKHRSurfaceFlag(bool state)
	{
		m_Information.disableKHRSurface = state;
		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::ToggleWindowingSurfaceFlag(bool state)
	{
		m_Information.disableWindowingSurface = state;
		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::SetHeadless()
	{
		m_Information.disableKHRSurface = true;
		m_Information.disableWindowingSurface = true;
		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::SetNotHeadless()
	{
		m_Information.disableKHRSurface = false;
		m_Information.disableWindowingSurface = false;
		return *this;
	}

	// Debug Messenger:
	VulkanInstanceBuilder& VulkanInstanceBuilder::UseDebugMessenger(bool value)
	{
		if (value)
			UseDefaultDebugCallback();

		m_Information.useDebugMessenger = value;
		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::UseDefaultDebugCallback()
	{
		AddValidationLayer();
		AddDebugUtilsExtension();

		m_Information.useDebugMessenger = true;
		m_Information.debugCallback = DefaultDebugCallback;
		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::SetDebugCallback(VulkanDebugCallback callback)
	{
		AddValidationLayer();
		AddDebugUtilsExtension();

		m_Information.useDebugMessenger = true;
		m_Information.debugCallback = callback;
		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::SetDebugMessengerSeverity(VulkanDebugSeverity severity)
	{
		m_Information.messageSeverity = severity;
		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::AddDebugMessengerSeverity(VulkanDebugSeverity severity)
	{
		m_Information.messageSeverity |= severity;
		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::SetDebugMessengerType(VulkanMessageType type)
	{
		m_Information.messageType = type;
		return *this;
	}

	VulkanInstanceBuilder& VulkanInstanceBuilder::AddDebugMessengerType(VulkanMessageType type)
	{
		m_Information.messageType = m_Information.messageType | type;
		return *this;
	}

	// Extensions:
	void VulkanInstanceBuilder::RequestSurfaceExtension(const std::vector<vk::ExtensionProperties>& supportedExtensions)
	{
		if (m_Information.disableKHRSurface)
			return;

		bool supportsKHRSurface = CheckExtensionSupport(supportedExtensions, SurfaceExtensionName);
		if (supportsKHRSurface)
			m_Information.extensions.push_back(SurfaceExtensionName);
		else
			TUR_LOG_CRITICAL("This device does not support the KHR Surface extension");
	}

	void VulkanInstanceBuilder::RequestWindowingExtensions(const std::vector<vk::ExtensionProperties>& supportedExtensions)
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
}