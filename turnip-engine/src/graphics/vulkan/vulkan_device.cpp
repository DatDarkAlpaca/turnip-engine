#include "pch.hpp"
#include "vulkan_device.hpp"
#include "vulkan_command_buffer.hpp"
#include "platform/vulkan_context.hpp"

#include "builders/instance_builder.hpp"
#include "builders/physical_device_builder.hpp"
#include "builders/logical_device_builder.hpp"
#include "builders/vma_allocator_builder.hpp"
#include "builders/swapchain_builder.hpp"
#include "builders/frame_data_builder.hpp"
#include "builders/sync_builder.hpp"

namespace tur::vulkan
{
	void GraphicsDeviceVulkan::initialize_impl(NON_OWNING Window* window, const ConfigData& configData)
	{
		const auto& vulkanConfig = configData.vulkanConfiguration;

		// Instance:
		initialize_instance(m_State, configData);

		// Surface:
		m_State.surface = get_vulkan_surface(m_State.instance, window);

		// Physical Device:
		initialize_physical_device(m_State, vulkanConfig.physicalDeviceRequirements);

		// Logical Device:
		initialize_logical_device(m_State, vulkanConfig);
	
		// VMA Allocator:
		initialize_vma_allocator(m_State);

		// Swapchain:
		initialize_swapchain(m_State, {});

		// Frame Data:
		initialize_frame_data(m_State);
	}
	void GraphicsDeviceVulkan::present_impl()
	{
		auto& graphicsQueue = m_State.queueList.get(QueueUsage::GRAPHICS);
		auto& presentQueue = m_State.queueList.get(QueueUsage::PRESENT);
		auto& frameDataHolder = m_State.frameDataHolder;
		const auto& frameData = frameDataHolder.get_frame_data();

		// Submit:
		vk::Semaphore signalSemaphores[] = { frameData.renderFinishedSemaphore };
		vk::Semaphore waitSemaphores[] = { frameData.imageAvailableSemaphore };
		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		vk::SubmitInfo submitInfo = {};
		{
			submitInfo.pWaitDstStageMask = waitStages;

			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;

			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &frameData.commandBuffer;
		}

		try
		{
			graphicsQueue.submit(submitInfo, frameData.recordingFence);
		}
		catch (vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to submit commands to the graphics queue. {}", err.what());
		}

		// Present:
		u32 imageIndices[] = { frameDataHolder.get_color_buffer() };
		vk::PresentInfoKHR presentInfo = {};
		{
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = &m_State.swapchain;
			presentInfo.pImageIndices = imageIndices;
		}

		try
		{
			presentQueue.presentKHR(presentInfo);
		}
		catch (vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to submit a present command to the present queue. {}", err.what());
		}

		frameDataHolder.increment_frame_count();

		// Acquire next image:
		/*m_State.frameDataHolder.increment_frame_count();
		m_State.logicalDevice.waitForFences(frameData.recordingFence, true, 1'000'000'000);
		m_State.logicalDevice.resetFences(frameData.recordingFence);

		auto image = m_State.logicalDevice.acquireNextImageKHR(m_State.swapchain, 1'000'000'000, frameData.imageAvailableSemaphore);
		m_CurrentColorBufferIndex = image.value;*/
	}

	CommandBufferVulkan GraphicsDeviceVulkan::create_command_buffer_impl()
	{
		return CommandBufferVulkan(this);
	}
}