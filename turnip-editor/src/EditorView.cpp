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

    {
        Entity entity = m_CurrentScene.NewEntity();
        entity.AddComponent<ComponentTransform>();
        entity.AddComponent<ComponentTexture>(m_Texture);
    }

    TofuRenderer::SetColor("#FAF4E8FF");
}

void EditorView::OnUpdate()
{
    TofuRenderer::Begin();

    auto view = m_CurrentScene.GetRegistry()->view<ComponentTransform, ComponentTexture>();
    for (auto&& [entity, transform, texture] : view.each())
    {
        auto useTranform = transform.transform;
        auto useTexture = texture.texture.lock();
        TofuRenderer::DrawQuad(useTranform, *useTexture.get(), *m_Shader);
    }

    TofuRenderer::End();
}

void EditorView::OnRenderGUI()
{
}

void EditorView::OnShutdown()
{
    m_Texture->Destroy();

    m_Shader->Destroy();
    delete m_Shader;
}
