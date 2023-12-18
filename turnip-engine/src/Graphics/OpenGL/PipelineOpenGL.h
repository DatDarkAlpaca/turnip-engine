#pragma once
#include "CommonGL.h"
#include "ShaderOpenGL.h"
#include "Graphics/Pipeline.h"

namespace
{
	inline void CheckProgramLinkErrors(tur::GLID program)
	{
		int success;
		char infoLog[1024];

		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, TUR_GL_LOG_BUFFER_SIZE, NULL, infoLog);
			TUR_LOG_ERROR("Failed to link program: {}", infoLog);
		}
	}
}

namespace tur
{
	class PipelineOpenGL : public Pipeline
	{
	public:
		explicit PipelineOpenGL(const PipelineDescriptor& descriptor);

	private:
		GLID m_ID = InvalidGLID;
	};
}