#pragma once
#include <TurnipEngine.h>

using namespace tur;

class EditorView : public IView
{
public:
    void OnInitialize() override;

    void OnUpdate() override;

    void OnRenderGUI();

    void OnShutdown() override;

private:
    Texture m_Texture;
    Mesh m_Mesh;
    Shader* m_Shader = nullptr;
};