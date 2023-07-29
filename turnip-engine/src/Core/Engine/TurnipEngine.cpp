#include "pch.h"
#include "TurnipEngine.h"

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
        api->Shutdown();
    }

    void TurnipEngine::Run()
    {
        if (!m_State.initialized)
        {
            TUR_CORE_ERROR("Failed to initialize the required subsystems.");
            return;
        }

        Initialize();

        while (window->IsOpen())
        {
            window->PollEvents();

            OnUpdate();

            OnRenderGUI();

            /* window.SwapBuffers(); */
        }

        Shutdown();
    }

    void TurnipEngine::OnEvent(IEvent& event)
    {
        Subscriber subscriber(event);

        /*
        subscriber.SubscribeTo<WindowResizeEvent>([&](WindowResizeEvent& event) -> bool {
            window.SetViewport({ event.width, event.height });
            return false;
        });
        */

        for (auto& view : viewQueue)
            view->OnEvent(event);
    }

    void TurnipEngine::OnUpdate()
    {
        for (auto& view : viewQueue)
            view->OnUpdate();
    }

    void TurnipEngine::OnRenderGUI()
    {
        for (auto& view : viewQueue)
            view->OnRenderGUI();
    }

    void TurnipEngine::SwapGraphicsSystem(GraphicsAPI_Type graphicsAPI, const WindowProperties& properties)
    {
        TUR_CORE_DEBUG("Selected Graphics API: {}", GetGraphicsAPI_String(graphicsAPI));

        api.reset();
        api = SelectGraphicsAPI(graphicsAPI);
        if (!api)
            return;

        std::unique_ptr<IGraphicsAPI_Loader> loader = SelectGraphicsAPI_Loader(graphicsAPI);
        loader->PreInitialize();

        window.reset();
        window = std::make_unique<Window>(properties);

        loader->PostInitialize(window.get());
    }

    void TurnipEngine::Setup()
    {
        InitializeLogger();

        // Graphics Systems:
        SwapGraphicsSystem(GraphicsAPI_Type::DEFAULT);

        // State Setup:
        m_State.initialized = true;
    }
}