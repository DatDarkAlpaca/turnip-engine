#include "pch.hpp"
#include "InstanceBuilder.hpp"
#include "Platform/Platform.hpp"

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
	InstanceBuilder::InstanceBuilder()
	{
#ifdef TUR_DEBUG
		AddValidationLayer();
		AddDebugUtilsExtension();
		UseDefaultDebugCallback();
#endif
	}

	std::optional<InstanceObject> InstanceBuilder::Build()
	{
		InstanceObject instanceObject;

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
			instanceObject.instance = vk::createInstance(createInfo);
		}
		catch (const vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to create vulkan instance: {}", err.what());
			return std::nullopt;
		}

		// Messenger Creation:
		if (!m_Information.useDebugMessenger)
			return instanceObject;

		vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT(
			vk::DebugUtilsMessengerCreateFlagsEXT(),
			m_Information.messageSeverity,
			m_Information.messageType,
			m_Information.debugCallback,
			nullptr
		);

		vk::DispatchLoaderDynamic DLDI(instanceObject.instance, vkGetInstanceProcAddr);
		instanceObject.debugMessenger = instanceObject.instance.createDebugUtilsMessengerEXT(debugCreateInfo, nullptr, DLDI);
		instanceObject.DLDI = DLDI;

		return instanceObject;
	}

	void InstanceBuilder::DisplayVulkanAPIVersion() const
	{
		// TODO: move to a graphics backend
		auto variant = VK_API_VERSION_VARIANT(m_Information.apiVersion);
		auto major = VK_API_VERSION_MAJOR(m_Information.apiVersion);
		auto minor = VK_API_VERSION_MINOR(m_Information.apiVersion);
		auto patch = VK_API_VERSION_PATCH(m_Information.apiVersion);

		TUR_LOG_DEBUG("Vulkan Version: {}.{}.{} - {}", major, minor, patch, variant);
	}

	// Application Information:
	InstanceBuilder& InstanceBuilder::SetAppName(const std::string& applicationName)
	{
		m_Information.applicationName = applicationName;
		return *this;
	}
	
	InstanceBuilder& InstanceBuilder::SetEngineName(const std::string& engineName)
	{
		m_Information.engineName = engineName;
		return *this;
	}

	InstanceBuilder& InstanceBuilder::SetApplicationVersion(uint32_t applicationVersion)
	{
		m_Information.applicationVersion = applicationVersion;
		return *this;
	}
	
	InstanceBuilder& InstanceBuilder::SetApplicationVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t variant)
	{
		m_Information.applicationVersion = VK_MAKE_API_VERSION(variant, major, minor, patch);
		return *this;
	}

	InstanceBuilder& InstanceBuilder::SetEngineVersion(uint32_t applicationVersion)
	{
		m_Information.engineVersion = applicationVersion;
		return *this;
	}
	
	InstanceBuilder& InstanceBuilder::SetEngineVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t variant)
	{
		m_Information.engineVersion = VK_MAKE_API_VERSION(variant, major, minor, patch);
		return *this;
	}

	InstanceBuilder& InstanceBuilder::SetAPIVersion(uint32_t applicationVersion)
	{
		m_Information.apiVersion = applicationVersion;
		return *this;
	}
	
	InstanceBuilder& InstanceBuilder::SetAPIVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t variant)
	{
		m_Information.apiVersion = VK_MAKE_API_VERSION(variant, major, minor, patch);
		return *this;
	}

	// Layers & Extensions:
	InstanceBuilder& InstanceBuilder::AddLayer(const char* layerName)
	{
		m_Information.layers.push_back(layerName);
		return *this;
	}

	InstanceBuilder& InstanceBuilder::AddValidationLayer()
	{
		m_Information.layers.push_back(ValidationLayerName);
		m_Information.addValidationLayer = true;
		return *this;
	}

	InstanceBuilder& InstanceBuilder::AddLayers(const std::vector<std::string>& layers)
	{
		for (const auto& layer : layers)
			m_Information.layers.push_back(layer.c_str());

		return *this;
	}

	InstanceBuilder& InstanceBuilder::AddExtension(const char* extensionName)
	{
		m_Information.extensions.push_back(extensionName);
		return *this;
	}

	InstanceBuilder& InstanceBuilder::AddDebugUtilsExtension()
	{
		m_Information.extensions.push_back(DebugUtilsExtensionName);
		m_Information.addDebugExtensions = true;
		return *this;
	}
	
	InstanceBuilder& InstanceBuilder::AddExtensions(const std::vector<std::string>& extensions)
	{
		for (const auto& extension : extensions)
			m_Information.extensions.push_back(extension.c_str());

		return *this;
	}

	InstanceBuilder& InstanceBuilder::ToggleKHRSurfaceFlag(bool state)
	{
		m_Information.disableKHRSurface = state;
		return *this;
	}

	InstanceBuilder& InstanceBuilder::ToggleWindowingSurfaceFlag(bool state)
	{
		m_Information.disableWindowingSurface = state;
		return *this;
	}

	InstanceBuilder& InstanceBuilder::SetHeadless()
	{
		m_Information.disableKHRSurface = true;
		m_Information.disableWindowingSurface = true;
		return *this;
	}

	InstanceBuilder& InstanceBuilder::SetNotHeadless()
	{
		m_Information.disableKHRSurface = false;
		m_Information.disableWindowingSurface = false;
		return *this;
	}

	// Debug Messenger:
	InstanceBuilder& InstanceBuilder::UseDebugMessenger(bool value)
	{
		if (value)
			UseDefaultDebugCallback();

		m_Information.useDebugMessenger = value;
		return *this;
	}

	InstanceBuilder& InstanceBuilder::UseDefaultDebugCallback()
	{
		AddValidationLayer();
		AddDebugUtilsExtension();

		m_Information.useDebugMessenger = true;
		m_Information.debugCallback = DefaultDebugCallback;
		return *this;
	}

	InstanceBuilder& InstanceBuilder::SetDebugCallback(VulkanDebugCallback callback)
	{
		AddValidationLayer();
		AddDebugUtilsExtension();

		m_Information.useDebugMessenger = true;
		m_Information.debugCallback = callback;
		return *this;
	}

	InstanceBuilder& InstanceBuilder::SetDebugMessengerSeverity(VulkanDebugSeverity severity)
	{
		m_Information.messageSeverity = severity;
		return *this;
	}

	InstanceBuilder& InstanceBuilder::AddDebugMessengerSeverity(VulkanDebugSeverity severity)
	{
		m_Information.messageSeverity |= severity;
		return *this;
	}

	InstanceBuilder& InstanceBuilder::SetDebugMessengerType(VulkanMessageType type)
	{
		m_Information.messageType = type;
		return *this;
	}

	InstanceBuilder& InstanceBuilder::AddDebugMessengerType(VulkanMessageType type)
	{
		m_Information.messageType = m_Information.messageType | type;
		return *this;
	}

	// Extensions:
	void InstanceBuilder::RequestSurfaceExtension(const std::vector<vk::ExtensionProperties>& supportedExtensions)
	{
		if (m_Information.disableKHRSurface)
			return;

		bool supportsKHRSurface = CheckExtensionSupport(supportedExtensions, SurfaceExtensionName);
		if (supportsKHRSurface)
			m_Information.extensions.push_back(SurfaceExtensionName);
		else
			TUR_LOG_CRITICAL("This device does not support the KHR Surface extension");
	}

	void InstanceBuilder::RequestWindowingExtensions(const std::vector<vk::ExtensionProperties>& supportedExtensions)
	{
		if (m_Information.disableWindowingSurface)
			return;

		auto availableExtensionNames = tur::platform::vulkan::GetVulkanExtensionPlatformSurfaceNames();
		for (const char* extensionName : availableExtensionNames)
		{
			if (!CheckExtensionSupport(supportedExtensions, extensionName))
				continue;

			m_Information.extensions.push_back(extensionName);
			return;
		}

		TUR_LOG_CRITICAL("This device does not support any valid surface extension");
	}
}