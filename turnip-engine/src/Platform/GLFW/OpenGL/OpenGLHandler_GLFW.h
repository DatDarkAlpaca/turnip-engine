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
            glfwWindowHint(GLFW_VERSION_MAJOR, state.openglVersionMajor);
            glfwWindowHint(GLFW_VERSION_MAJOR, state.openglVersionMinor);
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }

        static void Postload(Window& window)
        {
            GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window.Get());
            glfwMakeContextCurrent(glfwWindow);
        }

        static void Shutdown()
        {
            glfwTerminate();
        }
    };
}
#endif