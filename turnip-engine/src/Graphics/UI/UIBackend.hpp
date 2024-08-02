#pragma once
#include "Core/Config/ConfigData.hpp"
#include "Platform/Platform.hpp"
#include "Common.hpp"

namespace tur
{
	class UIBackend
	{
	public:
		virtual ~UIBackend() = default;

	public:
		virtual void Initialize(platform::Window& window) = 0;

	public:
		virtual void StartFrame() = 0;

		virtual void EndFrame() = 0;
	};

	tur_unique<UIBackend> CreateUIBackend(const ConfigData& config, platform::Window& window);
}