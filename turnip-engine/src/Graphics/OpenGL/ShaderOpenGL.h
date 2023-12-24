#pragma once
#include "CommonGL.h"
#include "Graphics/Shader.h"

#define TUR_GL_LOG_BUFFER_SIZE 1 << 11

namespace tur
{
	class ShaderOpenGL : public IShader
	{
	public:
		explicit ShaderOpenGL(const ShaderDescriptor& descriptor);

	public:
		GLID GetID() const { return m_ID; }

	private:
		GLID m_ID = InvalidGLID;
	};
}