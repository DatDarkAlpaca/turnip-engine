#include "pch.hpp"

using namespace tur;

int main()
{
	TurnipEngine engine;
	engine.initialize("res/engine_config.json");
	engine.run();
}
