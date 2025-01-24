#pragma once
#include <vulkan/vulkan.hpp>
#include "common.hpp"

namespace tur::vulkan
{
	struct FrameData
	{
		vk::CommandPool commandPool;
		vk::CommandBuffer commandBuffer;

		vk::Fence recordingFence;
		vk::Semaphore imageAvailableSemaphore, renderFinishedSemaphore;
	};

	struct FrameDataHolder
	{
	public:
		FrameDataHolder(u32 totalAllocatedFrames = 3)
			: m_TotalAllocatedFrames(totalAllocatedFrames)
		{
			m_FrameData.resize(m_TotalAllocatedFrames);
		}

	public:
		void increment_frame_count()
		{
			m_FrameNumber = (m_FrameNumber + 1) % m_TotalAllocatedFrames;
		}

	public:
		inline FrameData& get_frame_data()
		{
			return m_FrameData[m_FrameNumber];
		}

		inline std::vector<FrameData>& get_frames()
		{
			return m_FrameData;
		}

		inline u32 get_color_buffer() { return m_ColorBuffer; }
		inline void set_color_buffer(u32 value) { m_ColorBuffer = value; }

	private:
		std::vector<FrameData> m_FrameData;
		u32 m_TotalAllocatedFrames;
		u32 m_FrameNumber = 0;
		u32 m_ColorBuffer = 0;
	};
}