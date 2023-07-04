#include "pch.h"
#include "TurnipEngine.h"

#include "OpenGL/Buffer.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/Shader.h"

static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) 
{
    TUR_CORE_CRITICAL("GL: {}", message);
}

namespace tur
{
    TurnipEngine::TurnipEngine()
    {
        Setup();
    }

    TurnipEngine::~TurnipEngine()
    {
        window.Destroy();
        glfwTerminate();
    }

    void TurnipEngine::Run()
    {
        if (!m_State.initialized)
        {
            TUR_CORE_ERROR("Failed to initialize the required subsystems.");
            return;
        }

        Initialize();

        while (!window.ShouldClose())
        {
            window.PollEvents();

            for (auto& view : viewQueue)
                view->OnUpdate();
        
            window.SwapBuffers();
        }

        Shutdown();
    }

    void TurnipEngine::Setup()
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

        window.Initialize(800, 600, "Turnip Engine v0.1");

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            TUR_CORE_ERROR("Failed to initialize GLAD.");
            return;
        }

        glDebugMessageCallback(MessageCallback, nullptr);

        window.SetViewport({ 0, 0 });

        m_State.initialized = true;
    }
}