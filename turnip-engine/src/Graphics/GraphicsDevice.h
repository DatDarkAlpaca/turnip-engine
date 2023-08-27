#pragma once
#include "Platform.h"
#include "GraphicsAPI.h"
#include "Core/Engine/TurnipEngineState.h"

namespace tur
{
    class GraphicsDevice
    {
    public:
        void Initialize(const TurnipEngineState& state, GraphicsAPI api)
        {
            if (m_Initialized)
                Shutdown();

            m_API = api;

            switch (api)
            {
                case GraphicsAPI::OPENGL:
                {
                    OpenGLHandler::Preload(state);
                } break;

                default:
                {
                    TUR_CORE_CRITICAL("Invalid Graphics API selected during graphics initialization.");
                    return;
                } break;
            }

            m_Initialized = true;
        }

        void Link(Window& window)
        {
            switch (m_API)
            {
                case GraphicsAPI::OPENGL:
                {
                    OpenGLHandler::Postload(window);
                } break;

                default:
                {
                    TUR_CORE_CRITICAL("Invalid Graphics API selected during graphics link stage.");
                    return;
                } break;
            }
        }

        void Shutdown()
        {
            m_Initialized = false;

            switch (m_API)
            {
                case GraphicsAPI::OPENGL:
                {
                    OpenGLHandler::Shutdown();
                } break;

                default:
                {
                    TUR_CORE_CRITICAL("Invalid Graphics API selected during shutdown.");
                    return;
                } break;
            }
        }

    public:
        void Swapbuffers(Window& window)
        {
            switch (m_API)
            {
                case GraphicsAPI::OPENGL: 
                {
                    OpenGLHandler::Swapbuffers(window);
                } break;

                default:
                {
                    TUR_CORE_CRITICAL("Invalid Graphics API selected during Swapbuffer call.");
                    return;
                } break;
            }
        }

    public:
        GraphicsAPI m_API = GraphicsAPI::NONE;
        bool m_Initialized = false;
    };
}