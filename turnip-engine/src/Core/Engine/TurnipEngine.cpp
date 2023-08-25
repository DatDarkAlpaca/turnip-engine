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
        }

        Shutdown();
    }

    void TurnipEngine::OnEvent(Event& event)
    {
        Subscriber subscriber(event);
        subscriber.SubscribeTo<WindowResizeEventData>([&](WindowResizeEventData* data) -> bool {
            TUR_CORE_INFO("{}, {}", data->width, data->height);
            return false;
        });
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

    void TurnipEngine::Setup()
    {
        InitializeLogger();

        if (!windowLoader.Preload())
            return;

        window = std::make_unique<Window>(WindowProperties());
        window->SetEventCallback(BIND_1(&TurnipEngine::OnEvent, this));
        windowLoader.PostLoad();

        m_State.initialized = true;
    }
}