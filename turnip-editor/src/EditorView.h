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
    AssetLibrary<Shader> m_ShaderLibrary;
    AssetLibrary<Texture> m_TextureLibrary;
};