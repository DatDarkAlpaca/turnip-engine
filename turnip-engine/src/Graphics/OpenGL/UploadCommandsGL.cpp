#include "pch.h"
#include "RenderDeviceGL.h"
#include "UploadCommandsGL.h"

namespace tur::gl
{
	UploadCommandsGL::UploadCommandsGL(NON_OWNING RenderDeviceGL* renderDevice)
		: r_RenderDevice(renderDevice)
	{

	}

	void UploadCommandsGL::UploadBuffer(BufferHandle buffer, void* data, size_t dataSize)
	{
		/* Dummy */
	}
}
