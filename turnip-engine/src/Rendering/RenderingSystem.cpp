#include "pch.h"
#include "RenderingSystem.h"

#include "Platform/OpenGL/Diagnostics.h"

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
                m_RenderDevice = tur::MakeUnique<RenderDeviceGL>();
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
