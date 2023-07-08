#include <TurnipEngine.h>
#include <Entry.h>
#include "EditorView.h"

using namespace tur;

class TurnipEditor : public TurnipEngine
{
public:
    void Initialize() override 
    {
        TUR_CORE_INFO("Editor initialized.");

        window.Resize({ 640, 480 });
        window.Rename("Turnip Editor v1.0");

        viewQueue.AddView(new EditorView());
    }

    void Shutdown() override
    {
        TUR_CORE_INFO("Editor shutdown.");
    }
};

CREATE_APPLICATION(TurnipEditor);
