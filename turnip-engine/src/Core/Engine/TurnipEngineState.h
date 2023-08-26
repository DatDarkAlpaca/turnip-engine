#pragma once
#include "Graphics/GraphicsAPI.h"

namespace tur
{
	struct TurnipEngineState
	{
		bool initialized = false;

		GraphicsAPI defaultGraphicsAPI = GraphicsAPI::OPENGL;
		int openglVersionMajor = 4;
		int openglVersionMinor = 1;
	};
}