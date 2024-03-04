#pragma once
#include "RenderCommands.h"
#include "Resource/Resources.h"

namespace tur
{
	class UploadCommands : public RenderCommands
	{
	public:
		virtual void UploadBuffer(BufferHandle buffer, void* data, size_t dataSize) = 0;
	};
}