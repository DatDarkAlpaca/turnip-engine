#include "pch.h"
#include "RenderDeviceVK.h"
#include "UploadCommandsVK.h"

#include "Builders/CommandBuilder.h"
#include "Builders/BufferBuilder.h"

namespace tur::vulkan
{
	UploadCommandsVK::UploadCommandsVK(NON_OWNING RenderDeviceVK* renderDevice)
		: r_RenderDevice(renderDevice)
	{
		CommandBufferBuilder builder;
		builder.SetArguments(r_RenderDevice->logicalDevice, r_RenderDevice->commandPool);
		auto commandBuffer = builder.Build();

		if (!commandBuffer.has_value())
			TUR_LOG_ERROR("Failed to create upload command buffer");

		m_CommandBuffer = commandBuffer.value();
	}

	void UploadCommandsVK::UploadBuffer(BufferHandle buffer, void* data, size_t dataSize)
	{
		/* Implement */
	}
}

