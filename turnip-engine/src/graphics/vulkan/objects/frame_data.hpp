#pragma once
#include <vulkan/vulkan.hpp>
#include "common.hpp"

namespace tur::vulkan
{
	struct FrameData
	{
		vk::CommandPool commandPool;
		vk::CommandBuffer commandBuffer;
	};

	struct FrameDataHolder
	{
	public:
		FrameDataHolder(u32 totalAllocatedFrames = 3)
			: m_TotalAllocatedFrames(totalAllocatedFrames)
		{

		}

	public:
		inline FrameData& get_frame_data()
		{
			return m_FrameData[m_FrameNumber % m_TotalAllocatedFrames];
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