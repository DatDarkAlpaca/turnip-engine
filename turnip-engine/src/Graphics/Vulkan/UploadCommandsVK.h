#pragma once
#include "Common.h"
#include "Rendering/UploadCommands.h"

#include <glad/glad.h>

namespace tur::vulkan
{
	class RenderDeviceVK;

	class UploadCommandsVK : public UploadCommands
	{
	public:
		explicit UploadCommandsVK(NON_OWNING RenderDeviceVK* renderDevice);

	public:
		void UploadBuffer(BufferHandle buffer, void* data, size_t dataSize) override;

	private:
		NON_OWNING RenderDeviceVK* r_RenderDevice;

	private:
		vk::CommandBuffer m_CommandBuffer = { };
	};
}