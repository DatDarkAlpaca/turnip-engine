#include "pch.h"
#include "TurnipEngine.h"

static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) 
{
    TUR_CORE_CRITICAL("GL: {}", message);
}

namespace tur
{
    TurnipEngine::TurnipEngine()
    {
        TUR_ASSERT(!s_Instance, "The engine instance already exists.");
        s_Instance = this;

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

            imguiView->Begin();

            for (auto& view : viewQueue)
                view->OnRenderGUI();

            imguiView->End();
        
            window.SwapBuffers();
        }

        Shutdown();
    }

    void TurnipEngine::Setup()
    {
        InitializeLogger();

        glfwWindowHint(GLFW_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_VERSION_MINOR, 1);
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

        window.SetViewport({ 800, 600 });

        imguiView = new ImGuiView;
        viewQueue.Push(imguiView);

        m_State.initialized = true;
    }
}