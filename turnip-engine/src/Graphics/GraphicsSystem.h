#pragma once
#include "Common.h"
#include "Platform/Platform.h"
#include "Graphics/GraphicsBackend.h"
#include "Core/Window/WindowProperties.h"

#include "Graphics/Vulkan/VulkanInitializer.h"

namespace tur
{
	class GraphicsSystem : public Singleton<GraphicsSystem>
	{
	public:
		void Initialize(const FnEventCallback& windowCallback);

	public:
		void SetupWindow(const WindowProperties& properties);

	public:
		void SelectGraphicsBackend(BackendType type, const BackendProperties& properties);

		template<typename Initializer>
		void InitializeBackend()
		{
			tur_unique<Initializer> initializer = tur::MakeUnique<Initializer>(m_Backend.get());
		}

	public:
		tur_unique<Window>& GetWindow() { return m_Window; }

		tur_shared<IGraphicsBackend>& GetBackend() { return m_Backend; }

	private:
		tur_shared<IGraphicsBackend> m_Backend;
		tur_unique<Window> m_Window;
	};
}