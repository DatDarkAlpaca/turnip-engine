#pragma once
#include "Common.h"
#include "Rendering/UploadCommands.h"

#include <glad/glad.h>

namespace tur::gl
{
	class RenderDeviceGL;

	class UploadCommandsGL : public UploadCommands
	{
	public:
		explicit UploadCommandsGL(NON_OWNING RenderDeviceGL* renderDevice);

	public:
		void UploadBuffer(BufferHandle buffer, void* data, size_t dataSize) override;

	private:
		NON_OWNING RenderDeviceGL* r_RenderDevice;
	};
}