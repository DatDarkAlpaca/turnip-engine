#include "pch.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include "graphics/vulkan/vulkan_constants.hpp"
#include "graphics/vulkan/vulkan_device.hpp"
#include "graphics/system.hpp"

#include "platform/vulkan_context.hpp"
#include "platform/glfw/window_glfw.hpp"

namespace tur::vulkan
{
	void initialize_vulkan_windowing(Window* window, const WindowProperties& properties, const GraphicsSpecification& specification)
	{
		if (!glfwVulkanSupported())
			TUR_LOG_CRITICAL("GLFW Vulkan is not supported in this machine");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		
		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
		glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		initialize_window(window, properties);
	}
	vk::DescriptorPool initialize_vulkan_gui(vulkan::GraphicsDeviceVulkan* device)
	{
		auto& state = device->get_state();

		vk::DescriptorPoolSize poolSizes[] = 
		{
			{ vk::DescriptorType::eCombinedImageSampler, 5 },
		};

		// TODO: configuration
		vk::DescriptorPoolCreateInfo poolInfo = {};
		{
			poolInfo.maxSets = 5;
			poolInfo.poolSizeCount = static_cast<u32>(std::size(poolSizes));
			poolInfo.pPoolSizes = poolSizes;
			poolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
		}

		vk::DescriptorPool descriptorPool;
		{
			try
			{
				descriptorPool = state.logicalDevice.createDescriptorPool(poolInfo);
			}
			catch (vk::SystemError)
			{
				TUR_LOG_CRITICAL("Failed to create imgui descriptor pool");
			}
		}
	
		ImGui_ImplGlfw_InitForVulkan(device->get_window()->window, true);

		ImGui_ImplVulkan_InitInfo initInfo = {};

		static VkFormat formats[] = { (VkFormat)state.swapchainFormat.format };
		{
			initInfo.Instance = state.instance;
			initInfo.PhysicalDevice = state.physicalDevice;
			initInfo.Device = state.logicalDevice;
			initInfo.QueueFamily = state.queueList.get_family_index(QueueUsage::GRAPHICS);
			initInfo.Queue = state.queueList.get(QueueUsage::GRAPHICS);
			initInfo.DescriptorPool = descriptorPool;
			initInfo.MinImageCount = 3;
			initInfo.ImageCount = 3;
			initInfo.UseDynamicRendering = true;
			initInfo.PipelineRenderingCreateInfo = {};
			{
				initInfo.PipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
				initInfo.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
				initInfo.PipelineRenderingCreateInfo.pColorAttachmentFormats = formats;
			}
			initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		}

		ImGui_ImplVulkan_Init(&initInfo);

		ImGui_ImplVulkan_CreateFontsTexture();

		return descriptorPool;
	}
	void shutdown_vulkan_gui(vk::Device device, vk::DescriptorPool descriptorPool)
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		device.destroyDescriptorPool(descriptorPool);
	}

	void begin_vulkan_frame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void render_vulkan_frame(vk::CommandBuffer commandBuffer)
	{
		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
	}
	void end_vulkan_frame()
	{		
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	std::vector<const char*> get_windowing_vulkan_extensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		return extensions;
	}
	VkSurfaceKHR get_vulkan_surface(vk::Instance instance, Window* window)
	{
		VkSurfaceKHR surface;
		VkResult result = glfwCreateWindowSurface(instance, std::any_cast<GLFWwindow*>(window->window), nullptr, &surface);

		if (result == VK_SUCCESS)
			return surface;

		TUR_LOG_CRITICAL("Failed to create window surface using GLFW and Vulkan");
		return nullptr;
	}
	std::vector<const char*> get_vulkan_extension_platform_surface_names()
	{
#ifdef TUR_PLATFORM_WIN32
		return { tur::vulkan::WIN32_SurfaceExtensionName };
#elif defined(TUR_PLATFORM_LINUX)
		return {
			tur::vulkan::XCB_SurfaceExtensionName,
			tur::vulkan::XLIB_SurfaceExtensionName,
			tur::vulkan::WAYLAND_SurfaceExtensionName
		};
#elif defined(TUR_PLATFORM_APPLE)
		return { METAL_SurfaceExtensionName };
#elif defined(TUR_PLATFORM_ANDROID)
		return { ANDROID_SurfaceExtensionName };
#endif
	}
}