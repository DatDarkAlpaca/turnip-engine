#pragma once
#include "Core/Window/WindowProperties.h"
#include "Graphics/IGraphicsBackend.h"

namespace tur
{
	class BackendOpenGL final : public IGraphicsBackend
	{
	public:
		BackendOpenGL(const WindowProperties& usedProperties)
			: m_Properties(usedProperties)
		{

		}

	public:
		void Initialize(const BackendProperties& properties) override;

	private:
		const WindowProperties& m_Properties;

	private:
		constexpr static inline uint32_t DefaultVersionMajor = 3;
		constexpr static inline uint32_t DefaultVersionMinor = 3;
	};
}