#include "pch.h"
#include "TurnipEngine.h"

#include "OpenGL/Buffer.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/Shader.h"

#include "Logger/Logger.h"
#include "Assert/Assert.h"

static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) 
{
    TUR_CORE_CRITICAL("GL: {}", message);
}

namespace tur
{
    TurnipEngine::TurnipEngine()
    {
        InitializeSystems();
    }

    TurnipEngine::~TurnipEngine()
    {
        m_Window.Destroy();
        
        glfwTerminate();
    }

    void TurnipEngine::SetApplication(TurnipApplication* application)
    {
        m_Application = application;
        TUR_ASSERT(m_Application, "No application has been defined.");
    }

    void TurnipEngine::Run()
    {
        if (!m_Initialized)
        {
            TUR_CORE_ERROR("Failed to initialize the required subsystems.");
            return;
        }

        m_Application->Initialize();

        while (!m_Window.ShouldClose())
        {
            m_Window.PollEvents();

            m_Application->Update();

            m_Window.SwapBuffers();
        }

        m_Application->Shutdown();
    }

    void TurnipEngine::InitializeSystems()
    {
        InitializeLogger();

        glfwWindowHint(GLFW_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        TUR_WRAP_DEBUG(glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE));

        if (!glfwInit())
        {
            TUR_CORE_ERROR("Failed to initialize GLFW.");
            return;
        }

        m_Window.Initialize(800, 600, "Turnip Engine v0.1");

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            TUR_CORE_ERROR("Failed to initialize GLAD.");
            return;
        }

        glDebugMessageCallback(MessageCallback, nullptr);

        m_Window.SetViewport({ 800, 600 });

        m_Initialized = true;
    }
}