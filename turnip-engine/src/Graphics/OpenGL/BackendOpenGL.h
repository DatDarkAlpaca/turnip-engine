#pragma once
#include "Core/Window/WindowProperties.h"
#include "Graphics/GraphicsBackend.h"
#include "Platform/Platform.h"

#define OPENGL_BACKEND(OpenGLBackendSmartPtr) static_cast<BackendOpenGL*>(OpenGLBackendSmartPtr.get())

namespace tur
{
	class BackendOpenGL final : public IGraphicsBackend
	{
	public:
		BackendOpenGL(const BackendProperties& properties);

	public:
		void FinishSetup(tur_unique<Window>& window) override;

		void Present() override;

	public:
		tur_unique<IShader> CreateShader(const ShaderDescriptor& descriptor) override;

		tur_unique<IRenderpass> CreateRenderpass(const RenderpassDescriptor& descriptor) override { TUR_LOG_WARN("NOT IMPLEMENTED: BackendOpenGL::CreateRenderpass"); return nullptr; };

		tur_unique<IPipeline> CreatePipeline(const PipelineDescriptor& descriptor) override;

	private:
		BackendProperties m_Properties;
		NON_OWNING Window* m_Window = nullptr;

	private:
		constexpr static inline uint32_t DefaultVersionMajor = 3;
		constexpr static inline uint32_t DefaultVersionMinor = 3;
	};
}