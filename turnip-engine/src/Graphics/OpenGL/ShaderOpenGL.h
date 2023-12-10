#pragma once
#include "CommonGL.h"
#include "Graphics/Shader.h"

namespace tur
{
	class ShaderOpenGL : public Shader
	{
	public:
		explicit ShaderOpenGL(const std::vector<ShaderDescriptor>& shaderDescriptors);

	private:
		GLID m_ID = InvalidGLID;
	};
}