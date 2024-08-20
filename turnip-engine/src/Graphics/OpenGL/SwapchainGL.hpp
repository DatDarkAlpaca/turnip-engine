#pragma once
#include "CommonGL.hpp"
#include "Graphics/Swapchain.hpp"

#include "Platform/Platform.hpp"

namespace tur::gl
{
	class SwapchainGL : public ISwapchain
	{
	public:
		SwapchainGL(platform::Window& window)
			: r_Window(window)
		{

		}

	public:
		void Present() override
		{
			platform::gl::SwapBuffers(r_Window);
		}

	private:
		platform::Window& r_Window;
	};
}