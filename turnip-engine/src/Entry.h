#pragma once
#include "Core/Engine/TurnipEngine.h"

extern tur::TurnipEngine* CreateApp();

int main(int argc, char** argv)
{
	using namespace tur;

	TurnipEngine* engine = CreateApp();
	
	engine->Run();
}