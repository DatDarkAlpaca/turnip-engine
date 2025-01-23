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
		inline FrameData& get_frame_data()
		{
			m_FrameNumber = (m_FrameNumber + 1) % m_TotalAllocatedFrames;
			return m_FrameData[m_FrameNumber];
		}

		inline std::vector<FrameData>& get_frames()
		{
			return m_FrameData;
		}

	private:
		std::vector<FrameData> m_FrameData;
		u32 m_TotalAllocatedFrames;
		u32 m_FrameNumber = 0;
	};
}