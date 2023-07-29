#include <TurnipEngine.h>
#include <Entry.h>

using namespace tur;

class TurnipEditor : public TurnipEngine
{
public:
    void Initialize() override 
    {
        TUR_CORE_INFO("Editor initialized.");

        SwapGraphicsSystem(GraphicsAPI_Type::NONE);
    }

    void Shutdown() override
    {
        TUR_CORE_INFO("Editor shutdown.");
    }
};

CREATE_APPLICATION(TurnipEditor);
