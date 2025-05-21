#include "pch.hpp"
#include <turnip_engine.hpp>

#include "views/project/project_view.hpp"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
using namespace tur;

int main()
{
	TurnipEngine engine;
	engine.initialize("res/engine_config.json");
	engine.add_view(tur::make_unique<ProjectView>());

	engine.run();

	return 0;
}
