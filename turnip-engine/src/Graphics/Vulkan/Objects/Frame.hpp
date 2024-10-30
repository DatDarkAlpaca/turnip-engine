#pragma once
#include <vulkan/vulkan.hpp>
#include "Renderpass.hpp"

namespace tur::vulkan
{
	struct Frame
	{
		vk::Image image;
		vk::ImageView view;
		vk::Framebuffer framebuffer;
		vk::CommandBuffer commandBuffer;

		vk::Fence inFlightFence;
		vk::Semaphore imageAvailableSemaphore, renderFinishedSemaphore;
	};

	struct Frames
	{
	public:
		void Add(const Frame& frame)
		{
			frames.push_back(frame);
		}

	public:
		RenderpassObject renderpassObject;

		std::vector<Frame> frames;
		uint32_t currentFrame = 0;
	};
}