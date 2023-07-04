#include "EditorView.h"

void EditorView::OnInitialize()
{
    m_Shader = new Shader({
                { "res/basic.vert", ShaderType::TUR_SHADER_VERTEX },
                { "res/basic.frag", ShaderType::TUR_SHADER_FRAGMENT },
        });

    m_Texture.Initialize("res/turnip.png");

    float width = 100 / 800.f, height = 100 / 600.f;
    std::vector<Vertex> vertices
    {
        { {  width, height, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 1.0f, 1.0f, 0.5f, 1.0f }, { 1.0f, 1.0f } },
        { { -width,  height, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 0.4f, 1.0f, 0.2f, 1.0f }, { 0.0f, 1.0f } },
        { { -width, -height, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 0.5f, 1.0f, 0.5f, 1.0f }, { 0.0f, 0.0f } },
        { {  width, -height, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 1.0f, 0.6f, 0.3f, 1.0f }, { 1.0f, 0.0f } }
    };
    std::vector<U32> indices { 0, 1, 2, 2, 3, 0 };

    m_Mesh.Initialize(vertices, indices);

    TofuRenderer::SetColor({ 250, 244, 232, 255 });
}

void EditorView::OnUpdate()
{
    TofuRenderer::Begin();

    TofuRenderer::DrawMesh(m_Mesh, m_Texture, *m_Shader);

    TofuRenderer::End();
}

void EditorView::OnRenderGUI()
{
}

void EditorView::OnShutdown()
{
    m_Texture.Destroy();

    m_Shader->Destroy();
    delete m_Shader;
}
