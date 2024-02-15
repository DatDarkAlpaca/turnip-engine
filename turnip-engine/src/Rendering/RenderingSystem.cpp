#include "pch.h"
#include "RenderingSystem.h"

#include "Platform/GLFW/OpenGL/SetupOpenGL.h"
#include "Platform/OpenGL/Diagnostics.h"
#include "Platform/OpenGL/RenderDeviceGL.h"
#include "Platform/OpenGL/RenderContextGL.h"

#include "Platform/Vulkan/RenderDeviceVK.h"
#include "Platform/GLFW/Vulkan/SetupVulkan.h"

namespace tur
{
    void RenderingSystem::Configure(Window& window, const GraphicsSpecification& specification)
    {
        s_API = specification.api;

        switch (specification.api)
        {
            case GraphicsAPI::OPENGL:
            {
                platform::SetupOpenGL(window, specification);
                m_RenderDevice = tur::MakeUnique<RenderDeviceGL>(&window);
                m_GraphicsContext = tur::MakeUnique<GraphicsRenderContextGL>(static_cast<RenderDeviceGL*>(m_RenderDevice.get()));
            } break;

            case GraphicsAPI::VULKAN:
            {
                platform::SetupVulkan();
                m_RenderDevice = tur::MakeUnique<RenderDeviceVK>(&window);
            } break;

            default: 
                TUR_LOG_CRITICAL("An invalid graphics API was provided to the render system specification. [Configure()]");
                break;
        }
    }

    void RenderingSystem::DisplayVersion()
    {
        switch (s_API)
        {
            case GraphicsAPI::OPENGL:
            {
                ShowOpenGLVersion();
            } break;

            default:
                TUR_LOG_CRITICAL("An invalid graphics API was provided to the render system specification. [DisplayVersion()]");
                break;
        }
    }
}
