#pragma once
#include "CommonGL.h"
#include "Platform/Platform.h"
#include "ShaderOpenGL.h"
#include "Graphics/Pipeline.h"

namespace tur
{
	class PipelineOpenGL : public IPipeline
	{
	public:
		explicit PipelineOpenGL(const PipelineDescriptor& descriptor, Window* window);

	public:
		void Bind();

	private:
		void SetupShaders(const PipelineDescriptor& descriptor);

		void SetupRasterizer(const PipelineDescriptor& descriptor);

		void SetupMultisampling(const PipelineDescriptor& descriptor);

		void SetupColorBlending(const PipelineDescriptor& descriptor);

	private:
		Window* m_Window;
		GLID m_ID = InvalidGLID;
	};
}