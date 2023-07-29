#pragma once
#include "Graphics/API/IGraphicsAPI.h"
#include "Graphics/API/IGraphicsAPI_Loader.h"

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