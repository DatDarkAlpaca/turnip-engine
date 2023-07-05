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
    Texture m_Texture;
    Mesh m_Mesh;
    Shader* m_Shader = nullptr;

    glm::vec3 m_Pos = glm::vec3(0.f);
    glm::vec4 m_Color = glm::vec4(0.f, 0.f, 0.f, 1.f);
};