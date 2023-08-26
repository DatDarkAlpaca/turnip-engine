#include <TurnipEngine.h>
#include <Entry.h>

using namespace tur;

struct View : public IView
{
public:
    View(Window& engine) : engine(engine) { }

public:
    void OnUpdate() override
    {
        glClearColor(1.0f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        OpenGLHandler::Swapbuffers(engine);
    }

private:
    Window& engine;
};

class TurnipEditor : public TurnipEngine
{
public:
    void Initialize() override 
    {
        TUR_CORE_INFO("Editor initialized.");
        viewQueue.AddView(new View(*window));
    }

    void Shutdown() override
    {
        TUR_CORE_INFO("Editor shutdown.");
    }
};

CREATE_APPLICATION(TurnipEditor);