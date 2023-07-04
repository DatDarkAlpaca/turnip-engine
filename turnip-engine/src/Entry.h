#pragma once
#include "Core/Logger/Logger.h"
#include "Core/Engine/TurnipEngine.h"

extern tur::TurnipEngine* CreateApp();

int main(int argc, char** argv)
{
	using namespace tur;

	TurnipEngine* engine = CreateApp();
	
	engine->Run();
}