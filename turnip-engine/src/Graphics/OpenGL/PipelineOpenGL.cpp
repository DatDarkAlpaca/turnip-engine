#include "pch.h"
#include "CommonGL.h"
#include "PipelineOpenGL.h"
#include "Converters.h"

namespace
{
    static void CheckProgramLinkErrors(tur::GLID program)
    {
        int success;
        char infoLog[TUR_GL_LOG_BUFFER_SIZE];

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program, TUR_GL_LOG_BUFFER_SIZE, NULL, infoLog);
            TUR_LOG_ERROR("Failed to link shader program: {}", infoLog);
        }
    }
}

namespace tur
{
    PipelineOpenGL::PipelineOpenGL(const PipelineDescriptor& descriptor, Window* window)
		: m_Window(window)
    {
		SetupShaders(descriptor);
		SetupRasterizer(descriptor);
		SetupMultisampling(descriptor);
		SetupColorBlending(descriptor);
    }

	void PipelineOpenGL::Bind()
	{
		glUseProgram(m_ID);
	}

	void PipelineOpenGL::SetupShaders(const PipelineDescriptor& descriptor)
	{
		// TODO: Allow compute shaders.

		ShaderOpenGL* vertexShader      = static_cast<ShaderOpenGL*>(descriptor.vertexShader);
		ShaderOpenGL* tessControlShader = static_cast<ShaderOpenGL*>(descriptor.tesselationControlShader);
		ShaderOpenGL* tessEvalShader    = static_cast<ShaderOpenGL*>(descriptor.tesselationEvaluationShader);
		ShaderOpenGL* geometryShader    = static_cast<ShaderOpenGL*>(descriptor.geometryShader);
		ShaderOpenGL* fragmentShader    = static_cast<ShaderOpenGL*>(descriptor.fragmentShader);

		GLID vertexShaderID = vertexShader->GetID();
		GLID fragmentShaderID = fragmentShader->GetID();

		TUR_ASSERT(vertexShaderID != InvalidGLID, "Invalid vertex shader provided.");
		TUR_ASSERT(fragmentShaderID != InvalidGLID, "Invalid fragment shader provided.");

		// Optional Shaders:
		GLID tessControlShaderID = InvalidGLID;
		GLID tessEvalShaderID = InvalidGLID;
		GLID geometryShaderID = InvalidGLID;

		if (tessControlShader)
			tessControlShaderID = tessControlShader->GetID();

		if (tessEvalShader)
			tessEvalShaderID = tessEvalShader->GetID();

		if (geometryShader)
			geometryShaderID = geometryShader->GetID();

		// Creation:
		m_ID = glCreateProgram();

		glAttachShader(m_ID, vertexShaderID);
		glAttachShader(m_ID, fragmentShaderID);

		if (tessControlShaderID != InvalidGLID)
			glAttachShader(m_ID, tessControlShaderID);

		if (tessEvalShaderID != InvalidGLID)
			glAttachShader(m_ID, tessEvalShaderID);

		if (geometryShaderID != InvalidGLID)
			glAttachShader(m_ID, geometryShaderID);

		glLinkProgram(m_ID);
		CheckProgramLinkErrors(m_ID);

		// Cleanup:
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		if (tessControlShaderID != InvalidGLID)
			glDeleteShader(tessControlShaderID);

		if (tessEvalShaderID != InvalidGLID)
			glDeleteShader(tessEvalShaderID);

		if (geometryShaderID != InvalidGLID)
			glDeleteShader(geometryShaderID);
	}

	void PipelineOpenGL::SetupRasterizer(const PipelineDescriptor& descriptor)
	{
		uint32_t polygonMode = GetPolygonMode(descriptor.polygonMode);
		uint32_t cullFace = GetCullMode(descriptor.cullMode);
		uint32_t frontFace = GetFrontFace(descriptor.frontFace);
		
		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

		if (descriptor.cullMode != CullMode::NONE)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(cullFace);
			glFrontFace(frontFace);
		}
	}

	void PipelineOpenGL::SetupMultisampling(const PipelineDescriptor& descriptor)
	{
		if (!descriptor.enableMultisampling)
			return;

		glEnable(GL_MULTISAMPLE);
		platform::SetWindowBufferSamples(descriptor.sampleCount);

		RecreateWindow(m_Window);
	}

	void PipelineOpenGL::SetupColorBlending(const PipelineDescriptor& descriptor)
	{
		if (descriptor.enableColorBlending)
		{
			glEnable(GL_BLEND);
			glBlendFuncSeparate(
				GetBlendFactor(descriptor.srcColorBlendFactor, BlendFactor::ONE),
				GetBlendFactor(descriptor.dstColorBlendFactor, BlendFactor::ZERO),
				GetBlendFactor(descriptor.srcAlphaColorBlendFactor, BlendFactor::ONE),
				GetBlendFactor(descriptor.dstAlphaColorBlendFactor, BlendFactor::ZERO)
			);

			glBlendEquationSeparate(
				GetBlendOperation(descriptor.colorBlendOp), 
				GetBlendOperation(descriptor.alphaBlendOp)
			);
		}

		if (descriptor.enableLogicOp)
		{
			glEnable(GL_COLOR_LOGIC_OP);
			glLogicOp(GetLogicOp(descriptor.logicOperation));
		}
	}
}

