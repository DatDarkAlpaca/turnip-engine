#include "EditorView.h"

struct Component { int a; };

void EditorView::OnInitialize()
{
    m_ShaderLibrary.LoadAsset("basic", std::make_shared<Shader>(std::vector<ShaderStructure>{
        { "res/basic.vert", ShaderType::TUR_SHADER_VERTEX },
        { "res/basic.frag", ShaderType::TUR_SHADER_FRAGMENT },
    }));
    
    std::shared_ptr<Texture> turnipTexture = std::make_shared<Texture>();
    turnipTexture->Initialize("res/turnip.png");

    auto texture = m_TextureLibrary.LoadAsset("turnip", std::move(turnipTexture));
    {
        Entity entity = m_CurrentScene.NewEntity();
        entity.AddComponent<ComponentTransform>();
        entity.AddComponent<ComponentTexture>(texture);
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

        auto shader = m_ShaderLibrary.Get("basic");
        TofuRenderer::DrawQuad(useTranform, *useTexture.get(), *shader);
    }

    TofuRenderer::End();
}

void EditorView::OnRenderGUI()
{
}

void EditorView::OnShutdown()
{
    m_TextureLibrary.Clear();
}
