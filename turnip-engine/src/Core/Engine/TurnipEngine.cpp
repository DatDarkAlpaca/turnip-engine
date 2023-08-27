#include "pch.h"
#include "TurnipEngine.h"
#include "Core/Window/WindowFactory.h"

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

            graphics.Swapbuffers(*window.get());
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

    void TurnipEngine::SwapGraphicsAPI(GraphicsAPI api)
    {
        window.reset();

        WindowFactory factory({});
        graphics.Initialize(m_State, api);

        window = factory.Create(BIND_1(&TurnipEngine::OnEvent, this));

        graphics.Link(*window.get());

        TUR_CORE_DEBUG("Selected Graphics API: {}", GetGraphicsAPIString(api));
    }

    void TurnipEngine::Setup()
    {
        InitializeLogger();
        SwapGraphicsAPI(m_State.defaultGraphicsAPI);

        m_State.initialized = true;
    }
}