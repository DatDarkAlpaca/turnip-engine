#include <TurnipEngine.h>
#include <Entry.h>

using namespace tur;

struct View : public IView
{
public:
    void OnUpdate() override
    {

    }
};

class TurnipEditor : public TurnipEngine
{
public:
    void Initialize() override 
    {
        TUR_CORE_INFO("Editor initialized.");
        viewQueue.AddView(new View());

        SwapGraphicsAPI(GraphicsAPI::OPENGL);
    }

    void Shutdown() override
    {
        TUR_CORE_INFO("Editor shutdown.");
    }
};

CREATE_APPLICATION(TurnipEditor);