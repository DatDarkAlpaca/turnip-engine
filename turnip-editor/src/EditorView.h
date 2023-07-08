#pragma once
#include <TurnipEngine.h>

using namespace tur;

class EditorView : public IView
{
public:
    void OnInitialize() override;

    void OnUpdate() override;

    void OnRenderGUI() override;

    void OnShutdown() override;

    void OnEvent(IEvent& event) override
    {
        Subscriber subscriber(event);
        subscriber.SubscribeTo<WindowResizeEvent>([](WindowResizeEvent& event) -> bool {
            TUR_CORE_INFO("Editor View: {}, {}", event.width, event.height);
            return false;
        });
    }

private:
    Scene m_CurrentScene;

private:
    std::shared_ptr<Texture> m_Texture;
    Shader* m_Shader = nullptr;
};