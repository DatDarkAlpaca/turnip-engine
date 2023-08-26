#pragma once
#include "pch.h"
#ifdef TUR_WINDOWING_GLFW
#include "Core/Engine/TurnipEngineState.h"
#include "Platform/GLFW/Window_GLFW.h"

namespace tur
{ 
    class OpenGLHandler
    {
    public:
        static void Preload(const TurnipEngineState& state)
        {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, state.openglVersionMajor);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, state.openglVersionMinor);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef TUR_PLATFORM_MACOS
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        }

        static void Postload(Window& window)
        {
            GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window.Get());
            glfwMakeContextCurrent(glfwWindow);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                TUR_CORE_CRITICAL("Failed to initialize GLAD.");
                return;
            }
        }

        static void Shutdown()
        {
            glfwTerminate();
        }

        static void Swapbuffers(Window& window)
        {
            glfwSwapBuffers(static_cast<GLFWwindow*>(window.Get()));
        }
    };
}
#endif