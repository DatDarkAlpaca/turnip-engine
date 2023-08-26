#pragma once
#include "pch.h"
#ifdef TUR_WINDOWING_WINDOWS
#include "Core/Engine/TurnipEngineState.h"
#include "Platform/Windows/Window_WIN32.h"

namespace tur
{
    inline PROC GetExtension(const char* extensionFunctionName)
    {
        return wglGetProcAddress(extensionFunctionName);
    }

    class OpenGLHandler
    {
    public:
        static void Preload(const TurnipEngineState& state)
        {
            s_State = state;
        }

        static void Postload(Window& window)
        {
            HWND handle = window.GetHandle();
            HDC hdc = GetDC(handle);

            if (!hdc)
            {
                TUR_CORE_ERROR("Failed to get window device context.");
                return;
            }

            PIXELFORMATDESCRIPTOR pfd = {};
            pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
            pfd.nVersion = 1;
            pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            pfd.iPixelType = PFD_TYPE_RGBA;
            pfd.cColorBits = 32;
            pfd.iLayerType = PFD_MAIN_PLANE;

            int pixelFormat = ChoosePixelFormat(hdc, &pfd);
            if (!pixelFormat)
            {
                TUR_CORE_ERROR("Failed to find a suitable pixel format");
                return;
            }

            if (!SetPixelFormat(hdc, pixelFormat, &pfd))
            {
                TUR_CORE_ERROR("Failed to set the requested pixel format");
                return;
            }

            PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 0;

            // Temporary context:
            {
                HGLRC hglrc = wglCreateContext(hdc);
                wglMakeCurrent(hdc, hglrc);

                wglCreateContextAttribsARB = 
                    (PFNWGLCREATECONTEXTATTRIBSARBPROC)GetExtension("wglCreateContextAttribsARB");

                wglMakeCurrent(0, 0);
                wglDeleteContext(hglrc);
            }

            int attributes[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB,
                s_State.openglVersionMajor,
                WGL_CONTEXT_MINOR_VERSION_ARB,
                s_State.openglVersionMinor,
                WGL_CONTEXT_PROFILE_MASK_ARB,
                WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                WGL_CONTEXT_FLAGS_ARB,
                WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                0
            };

            HGLRC context = wglCreateContextAttribsARB(hdc, NULL, attributes);
            if(!context)
            {
                TUR_CORE_ERROR("Failed to create a valid OpenGL {}.{} context", 
                    s_State.openglVersionMajor, s_State.openglVersionMinor);
                return;
            }

            s_Window = &window;
        }
        
        static void Shutdown()
        {
            HWND handle = s_Window->GetHandle();

            HGLRC context = wglGetCurrentContext();
            HDC hDC = wglGetCurrentDC();


            wglMakeCurrent(NULL, NULL);

            if (context)
                wglDeleteContext(context);

            if (hDC)
                ReleaseDC(handle, hDC);

            PostQuitMessage(0);
        }

    private:
        static inline TurnipEngineState s_State;
        static inline Window* s_Window = nullptr;
    };
}
#endif