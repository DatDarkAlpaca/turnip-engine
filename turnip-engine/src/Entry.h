#pragma once
#include "Core/Logger/Logger.h"
#include "Core/Engine/TurnipEngine.h"
#include "Core/Application/TurnipApplication.h"

extern tur::TurnipApplication* CreateApp();

int main(int argc, char** argv)
{
	using namespace tur;

	TurnipEngine engine;

	TurnipApplication* application = CreateApp();
	engine.SetApplication(application);
	
	engine.Run();

	delete application;
}