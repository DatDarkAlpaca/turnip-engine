#include "EditorView.h"

void EditorView::OnInitialize()
{
    glm::vec2 windowSize = TurnipEngine::Get().GetWindow().GetSize();
    m_Camera.SetProjection(0.f, windowSize.x, windowSize.y, 0.f);

    m_ShaderLibrary.LoadAsset("basic", std::make_shared<Shader>(std::vector<ShaderStructure>{
        { "res/basic.vert", ShaderType::TUR_SHADER_VERTEX },
        { "res/basic.frag", ShaderType::TUR_SHADER_FRAGMENT },
    }));
    
    std::shared_ptr<Texture> turnipTexture = std::make_shared<Texture>();
    turnipTexture->Initialize("res/turnip.png");

    auto texture = m_TextureLibrary.LoadAsset("turnip", std::move(turnipTexture));
    {
        Transform transform;
        transform.scale = glm::vec3(40.f);
        transform.position = glm::vec3(100.f, 100.f, 0.f);

        Entity entity = m_CurrentScene.NewEntity();
        entity.AddComponent<ComponentTransform>(transform);
        entity.AddComponent<ComponentTexture>(texture);
    }

    TofuRenderer::SetColor("#FAF4E8FF");
}

void EditorView::OnUpdate()
{
    m_Camera.UpdateCamera();
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
}

void EditorView::OnRenderGUI()
{
    //ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    ImGui::Begin("Camera Controls");

    ImGui::SliderFloat3("Camera Position", &m_Camera.GetPosition()[0], -500.f, 500.f);
   
    ImGui::End();
}

void EditorView::OnShutdown()
{
    m_TextureLibrary.Clear();
}

void EditorView::OnEvent(IEvent& event)
{
    Subscriber subscriber(event);
    subscriber.SubscribeTo<WindowResizeEvent>([&](WindowResizeEvent& window) -> bool {
        m_Camera.SetProjection(0.f, (float)window.width, (float)window.height, 0.f);
        return false;
    });
}
