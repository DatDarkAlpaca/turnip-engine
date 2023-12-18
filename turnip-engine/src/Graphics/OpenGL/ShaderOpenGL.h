#pragma once
#include "CommonGL.h"
#include "Graphics/Shader.h"

namespace tur
{
	class ShaderOpenGL : public Shader
	{
		friend class BackendOpenGL;

	private:
		explicit ShaderOpenGL(const ShaderDescriptor& descriptor);

	private:
		GLID m_ID = InvalidGLID;
	};
}