#pragma once
#include "Graphics/API/IGraphicsAPI.h"

namespace tur
{
	class OGL_API : public IGraphicsAPI
	{
	public:
		OGL_API()
		{
			
		}

	public:
		void Shutdown() override
		{
			glfwTerminate();
		}
	};
}