#include "EditorView.h"

void EditorView::OnInitialize()
{
    glm::vec2 windowSize = TurnipEngine::Get().GetWindow().GetSize();
    m_Camera.SetProjection(glm::radians(90.f), windowSize.x / windowSize.y, 0.1f, 100.f);
    m_Camera.SetPosition({ 0, 8.f, 25.f });
    m_Camera.SetWindowSize(windowSize.x, windowSize.y);

    m_ShaderLibrary.LoadAsset("basic", std::make_shared<Shader>(std::vector<ShaderStructure>{
        { "res/basic.vert", ShaderType::TUR_SHADER_VERTEX },
        { "res/basic.frag", ShaderType::TUR_SHADER_FRAGMENT },
    }));
    
    std::shared_ptr<Texture> turnipTexture = std::make_shared<Texture>();
    turnipTexture->Initialize("res/turnip.png");

    auto texture = m_TextureLibrary.LoadAsset("turnip", std::move(turnipTexture));
    {
        Transform transform;
        transform.scale = glm::vec3(20.f);
        transform.position = glm::vec3(0.f, 0.f, 0.f);

        Entity entity = m_CurrentScene.NewEntity();
        entity.AddComponent<ComponentTransform>(transform);
        entity.AddComponent<ComponentTexture>(texture);
    }

    TofuRenderer::Initialize({true, true});
    TofuRenderer::SetColor("#FAF4E8FF");
}

void EditorView::OnUpdate()
{
    m_Camera.Update();
    TofuRenderer::Begin(&m_Camera);

    auto shader = m_ShaderLibrary.Get("basic");

    auto view = m_CurrentScene.GetRegistry()->view<ComponentTransform, ComponentTexture>();
    for (auto&& [entity, transform, texture] : view.each())
    {
        auto useTranform = transform.transform;
        auto useTexture = texture.texture.lock();
        
        TofuRenderer::DrawQuad(useTranform, *useTexture.get(), *shader);
    }

    TofuRenderer::End();
    Mouse::Clear(); // Todo: move somewhere appropriate in the engine.
}

void EditorView::OnRenderGUI()
{
    
}

void EditorView::OnShutdown()
{
    m_TextureLibrary.Clear();
}

void EditorView::OnEvent(IEvent& event)
{
    m_Camera.OnEvent(event);
}
