#pragma once
#include "Core/Window/WindowProperties.h"
#include "Graphics/IGraphicsBackend.h"
#include "Platform/Platform.h"

namespace tur
{
	class BackendOpenGL final : public IGraphicsBackend
	{
	public:
		BackendOpenGL(tur_unique<Window>& window)
			: r_Window(window)
		{

		}

	public:
		void Initialize(const BackendProperties& properties) override;

		void Present() override;

	private:
		tur_unique<Window>& r_Window;

	private:
		constexpr static inline uint32_t DefaultVersionMajor = 3;
		constexpr static inline uint32_t DefaultVersionMinor = 3;
	};
}