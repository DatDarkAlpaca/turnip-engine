#pragma once
#include "Core/Config/ConfigData.hpp"
#include "Graphics/GraphicsLayer.hpp"
#include "Platform/Platform.hpp"

namespace tur
{
	class GraphicsSystem
	{
	public:
		void Initialize(const ConfigData& configData, platform::Window& window);

	private:
		void InitializeOpenGL(const ConfigData& configData, platform::Window& window);

		void InitializeVulkan(const ConfigData& configData, platform::Window& window);

	public:
		tur_unique<IGraphicsLayer>& GraphicsLayer() { return m_GraphicsLayer; }

	private:
		tur_unique<IGraphicsLayer> m_GraphicsLayer;
	};
}