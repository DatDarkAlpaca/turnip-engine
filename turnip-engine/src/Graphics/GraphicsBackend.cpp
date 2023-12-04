#include "pch.h"
#include "GraphicsBackend.h"

#include "OpenGL/BackendOpenGL.h"
#include "Vulkan/BackendVulkan.h"

namespace tur
{
	tur_shared<IGraphicsBackend> MakeGraphicsBackend(BackendType type, const BackendProperties& properties)
	{
		switch (type)
		{
			case BackendType::OPENGL:
				return tur::MakeShared<BackendOpenGL>(properties);

			case BackendType::VULKAN:
				return tur::MakeShared<BackendVulkan>(properties);

			default:
				TUR_LOG_CRITICAL("Unsupported graphics API");
		}

		return nullptr;
	}
}