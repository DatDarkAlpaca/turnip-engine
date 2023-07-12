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

    void OnEvent(IEvent& event) override;

private:
    Scene m_CurrentScene;
    OrthographicCamera m_Camera;
    AssetLibrary<Shader> m_ShaderLibrary;
    AssetLibrary<Texture> m_TextureLibrary;
};