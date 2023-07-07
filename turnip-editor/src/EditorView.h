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

private:
    Scene m_CurrentScene;

private:
    std::shared_ptr<Texture> m_Texture;
    Shader* m_Shader = nullptr;
};