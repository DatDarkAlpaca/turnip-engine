#include "pch.h"
#include "RenderDevice.h"

#include "Graphics/OpenGL/RenderDeviceGL.h"
#include "Graphics/Vulkan/RenderDeviceVK.h"
#include "Graphics/Vulkan/VulkanInitializer.h"

namespace tur
{
    tur_unique<RenderDevice> RenderDevice::Create(Window& window, const GraphicsSpecification& specification, RenderInitializer* initializer)
	{
        tur_unique<RenderDevice> renderDevice;

        switch (specification.api)
        {
            case GraphicsAPI::OPENGL:
            {
                platform::SetupOpenGL(window, specification);
                renderDevice = tur::MakeUnique<gl::RenderDeviceGL>(&window);

                if (initializer)
                    initializer->Initialize(renderDevice.get());

            } break;

            case GraphicsAPI::VULKAN:
            {
                platform::SetupVulkan(window);
                renderDevice = tur::MakeUnique<vulkan::RenderDeviceVK>(&window);

                if (initializer)
                {
                    initializer->Initialize(renderDevice.get());
                    break;
                }

                vulkan::DefaultVulkanInitializer vulkanInitializer;
                vulkanInitializer.Initialize(renderDevice.get());
            } break;

            default:
                TUR_LOG_CRITICAL("An invalid graphics API was provided to the render system specification. [Configure()]");
                break;
        }

        return renderDevice;
	}
}

