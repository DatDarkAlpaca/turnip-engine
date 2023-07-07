#include "EditorView.h"

struct Component { int a; };

void EditorView::OnInitialize()
{
    m_Shader = new Shader({
         { "res/basic.vert", ShaderType::TUR_SHADER_VERTEX },
         { "res/basic.frag", ShaderType::TUR_SHADER_FRAGMENT },
        });

    m_Texture = std::make_shared<Texture>();
    m_Texture->Initialize("res/turnip.png");

    m_Texture1 = std::make_shared<Texture>();
    m_Texture->Initialize("res/turnip1.png");

    Entity entity = m_CurrentScene.NewEntity();
    entity.AddComponent<ComponentTransform>();
    entity.AddComponent<ComponentTexture>(m_Texture);

    Entity entity1 = m_CurrentScene.NewEntity();
    entity1.AddComponent<ComponentTransform>();
    entity1.AddComponent<ComponentTexture>(m_Texture1);

    TofuRenderer::SetColor("#FAF4E8FF");
}

void EditorView::OnUpdate()
{
    TofuRenderer::SetColor(m_Color);
    TofuRenderer::Begin();

    if (m_Show)
    {
        auto view = m_CurrentScene.GetRegistry()->view<ComponentTransform, ComponentTexture>();
        for (auto&& [entity, transform, texture] : view.each())
        {
            auto useTranform = transform.transform;
            auto useTexture = texture.texture.lock();
            TofuRenderer::DrawQuad(useTranform, *useTexture.get(), *m_Shader);
        }
    }
    

    TofuRenderer::End();
}

void EditorView::OnRenderGUI()
{
    ImGui::Begin("Color Picker");
    ImGui::ColorPicker4("Color Picker", &m_Color[0]);
    ImGui::End();

    ImGui::Begin("Position");
    ImGui::SliderFloat3("Position", &m_Pos[0], -1, 1);
    ImGui::End();
}

void EditorView::OnShutdown()
{
    m_Texture->Destroy();

    m_Shader->Destroy();
    delete m_Shader;
}
