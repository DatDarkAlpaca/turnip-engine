#include "pch.h"
#include "RenderingSystem.h"

#include "Platform/GLFW/OpenGL/SetupOpenGL.h"
#include "Graphics/OpenGL/Diagnostics.h"
#include "Graphics/OpenGL/RenderDeviceGL.h"
#include "Graphics/OpenGL/GraphicsCommandsGL.h"

#include "Platform/GLFW/Vulkan/SetupVulkan.h"
#include "Graphics/Vulkan/RenderDeviceVK.h"
#include "Graphics/Vulkan/GraphicsCommandsVK.h"

namespace tur
{
    void RenderingSystem::Configure(Window& window, const GraphicsSpecification& specification)
    {
        s_API = specification;

        switch (s_API.api)
        {
            case GraphicsAPI::OPENGL:
            {
                platform::SetupOpenGL(window, specification);
                m_RenderDevice = tur::MakeUnique<gl::RenderDeviceGL>(&window);
                m_GraphicsContext = tur::MakeUnique<gl::GraphicsRenderCommandsGL>(static_cast<gl::RenderDeviceGL*>(m_RenderDevice.get()));
            } break;

            case GraphicsAPI::VULKAN:
            {
                platform::SetupVulkan(window);
                m_RenderDevice = tur::MakeUnique<vulkan::RenderDeviceVK>(&window);
                m_GraphicsContext = tur::MakeUnique<vulkan::GraphicsRenderCommandsVK>(static_cast<vulkan::RenderDeviceVK*>(m_RenderDevice.get()));
            } break;

            default: 
                TUR_LOG_CRITICAL("An invalid graphics API was provided to the render system specification. [Configure()]");
                break;
        }
    }

    void RenderingSystem::DisplayVersion()
    {
        switch (s_API.api)
        {
            case GraphicsAPI::OPENGL:
            {
                gl::ShowOpenGLVersion();
            } break;

            case GraphicsAPI::VULKAN:
            {
                // TODO: Implement
            } break;

            default:
                TUR_LOG_CRITICAL("An invalid graphics API was provided to the render system specification. [DisplayVersion()]");
                break;
        }
    }
}
