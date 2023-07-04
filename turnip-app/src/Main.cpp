#include <TurnipEngine.h>

using namespace tur;

class ViewMain : public IView
{
public:
    void OnInitialize() override
    {
        m_Shader = new Shader({
             { "res/basic.vert", ShaderType::TUR_SHADER_VERTEX },
             { "res/basic.frag", ShaderType::TUR_SHADER_FRAGMENT },
        });

        m_Texture.Initialize("res/turnip1.png");

        float width = 100 / 800.f, height = 100 / 600.f;
        std::vector<Vertex> vertices
        {
            { {  width, height, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 1.0f, 1.0f, 0.5f, 1.0f }, { 1.0f,  1.0f } },
            { { -width,  height, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 0.4f, 1.0f, 0.2f, 1.0f }, {  0.0f,  1.0f } },
            { { -width, -height, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 0.5f, 1.0f, 0.5f, 1.0f }, {  0.0f,  0.0f } },
            { {  width, -height, 0.0f }, { 0.0f , 0.0f , 0.0f }, { 1.0f, 0.6f, 0.3f, 1.0f }, {  1.0f,  0.0f } }
        };
        std::vector<U32> indices { 0, 1, 2, 2, 3, 0 };

        m_Mesh.Initialize(vertices, indices);

        TofuRenderer::SetColor({ 250, 244, 232, 255 });
    };

    void OnShutdown() override 
    {
        m_Texture.Destroy();

        m_Shader->Destroy();
        delete m_Shader;
    };

    void OnUpdate() override 
    {
        TofuRenderer::Begin();

        TofuRenderer::DrawMesh(m_Mesh, m_Texture, *m_Shader);

        TofuRenderer::End();
    };

private:
    Texture m_Texture;
    Mesh m_Mesh;
    Shader* m_Shader = nullptr;
};

class TestApplication : public TurnipEngine
{
public:
    void Initialize() override 
    {
        TUR_CORE_INFO("Application initialized.");

        window.Resize({ 640, 480 });
        window.Rename("TestApplication v1.0");

        viewQueue.Push(new ViewMain());
    }

    void Shutdown() override
    {
        TUR_CORE_INFO("Application shutdown.");
    }
};

CREATE_APPLICATION(TestApplication);
