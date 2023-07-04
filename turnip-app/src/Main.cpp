#include <TurnipEngine.h>

using namespace tur;

class Test : public TurnipApplication
{
public:
    void Initialize() override
    {
        m_Shader = new Shader({
            { "res/basic.vert", ShaderType::TUR_SHADER_VERTEX },
            { "res/basic.frag", ShaderType::TUR_SHADER_FRAGMENT },
        });

        m_Texture.Initialize("res/turnip.png");

        float width = 100 / 800.f, height = 100 / 600.f;
        std::vector<Vertex> vertices
        {
            { {  width,  height, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 1.0f, 1.0f, 0.5f, 1.0f }, {  1.0f,  1.0f } },
            { { -width,  height, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 0.4f, 1.0f, 0.2f, 1.0f }, {  0.0f,  1.0f } },
            { { -width, -height, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 0.5f, 1.0f, 0.5f, 1.0f }, {  0.0f,  0.0f } },
            { {  width, -height, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 1.0f, 0.6f, 0.3f, 1.0f }, {  1.0f,  0.0f } }
        };
        std::vector<U32> indices { 0, 1, 2, 2, 3, 0 };

        m_Mesh.Initialize(vertices, indices);

        m_Renderer.SetColor({ 0.9803f, 0.9568f, 0.9098f, 1.f });
    }

    void Update() override
    {
        m_Renderer.Begin();

        m_Renderer.DrawMesh(m_Mesh, m_Texture, *m_Shader);

        m_Renderer.End();
    }

    void Shutdown() override
    {
        m_Shader->Destroy();
        delete m_Shader;
    }

private:
    TofuRenderer m_Renderer;
    Texture m_Texture;
    Mesh m_Mesh;
    Shader* m_Shader = nullptr;
};

CREATE_APPLICATION(Test);
