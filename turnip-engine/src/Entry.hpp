#pragma once
#include "Core/Memory/Memory.hpp"
#include "Core/Engine/TurnipEngine.hpp"

extern tur::tur_unique<tur::TurnipEngine> CreateApplication();

int main(int argc, char** argv)
{
	using namespace tur;

	tur_unique<TurnipEngine> engine = CreateApplication();
	engine->Run();
}