#include "pch.h"
#include "CommonGL.h"
#include "PipelineOpenGL.h"

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
	static constexpr uint32_t GetPolygonMode(PolygonMode polygonMode)
	{
		switch (polygonMode)
		{
			case PolygonMode::POINT:
				return GL_POINT;
			case PolygonMode::LINE:
				return GL_LINE;
			case PolygonMode::FILL:
				return GL_FILL;
			default:
			{
				TUR_LOG_ERROR("Invalid Polygon Mode. Using default: FILL");
				return GL_FILL;
			} break;
		}

		return 0;
	}

	static constexpr uint32_t GetCullMode(CullMode cullMode)
	{
		switch (cullMode)
		{
			case CullMode::FRONT:
				return GL_FRONT;
			case CullMode::BACK:
				return GL_BACK;
			case CullMode::FRONT_AND_BACK:
				return GL_FRONT_AND_BACK;
			default:
			{
				TUR_LOG_ERROR("Invalid Polygon Mode. Using default: FRONT");
				return GL_BACK;
			}
		}

		return 0;
	}

	static constexpr uint32_t GetFrontFace(FrontFace frontFace)
	{
		switch (frontFace)
		{
			case FrontFace::COUNTER_CLOCKWISE:
				return GL_CCW;
			case FrontFace::CLOCKWISE:
				return GL_CW;
			default:
			{
				TUR_LOG_ERROR("Invalid Front Face. Using default: CLOCKWISE");
				return GL_CW;
			} break;
		}

		return 0;
	}

	static constexpr uint32_t GetBlendFactor(BlendFactor blendFactor, BlendFactor fallback = BlendFactor::ONE)
	{
		switch (blendFactor)
		{
			case BlendFactor::ZERO:
				return GL_ZERO;
			case BlendFactor::ONE:
				return GL_ONE;
			case BlendFactor::SRC_COLOR:
				return GL_SRC_COLOR;
			case BlendFactor::ONE_MINUS_SRC_COLOR:
				return GL_ONE_MINUS_SRC_COLOR;
			case BlendFactor::DST_COLOR:
				return GL_DST_COLOR;
			case BlendFactor::ONE_MINUS_DST_COLOR:
				return GL_ONE_MINUS_DST_COLOR;
			case BlendFactor::SRC_ALPHA:
				return GL_SRC_ALPHA;
			case BlendFactor::ONE_MINUS_SRC_ALPHA:
				return GL_ONE_MINUS_SRC_ALPHA;
			case BlendFactor::DST_ALPHA:
				return GL_DST_ALPHA;
			case BlendFactor::ONE_MINUS_DST_ALPHA:
				return GL_ONE_MINUS_DST_ALPHA;
			case BlendFactor::CONSTANT_COLOR:
				return GL_CONSTANT_COLOR;
			case BlendFactor::ONE_MINUS_CONSTANT_COLOR:
				return GL_ONE_MINUS_CONSTANT_COLOR;
			case BlendFactor::CONSTANT_ALPHA:
				return GL_CONSTANT_ALPHA;
			case BlendFactor::ONE_MINUS_CONSTANT_ALPHA:
				return GL_ONE_MINUS_CONSTANT_ALPHA;
			case BlendFactor::SRC_ALPHA_SATURATE:
			{
				TUR_LOG_WARN("Unsupported blend factor: SRC_ALPHA_SATURATE. Using fallback");
				return GetBlendFactor(fallback);
			} break;
			case BlendFactor::SRC1_COLOR:
			{
				TUR_LOG_WARN("Unsupported blend factor: SRC1_COLOR. Using fallback");
				return GetBlendFactor(fallback);
			} break;
			case BlendFactor::ONE_MINUS_SRC1_COLOR:
			{
				TUR_LOG_WARN("Unsupported blend factor: ONE_MINUS_SRC1_COLOR. Using fallback");
				return GetBlendFactor(fallback);
			} break;
			case BlendFactor::SRC1_ALPHA:
			{
				TUR_LOG_WARN("Unsupported blend factor: SRC1_ALPHA. Using fallback");
				return GetBlendFactor(fallback);
			} break;
			case BlendFactor::ONE_MINUS_SRC1_ALPHA:
			{
				TUR_LOG_WARN("Unsupported blend factor: ONE_MINUS_SRC1_ALPHA. Using fallback");
				return GetBlendFactor(fallback);
			} break;

			default:
			{
				TUR_LOG_ERROR("Invalid Blend Factor. Using fallback.");
				return GetBlendFactor(fallback);
			} break;
		}
	}

	static constexpr uint32_t GetBlendOperation(BlendOperation blendOperation)
	{
		switch (blendOperation)
		{
			case BlendOperation::ADD:
				return GL_FUNC_ADD;
			case BlendOperation::SUBTRACT:
				return GL_FUNC_SUBTRACT;
			case BlendOperation::REVERSE_SUBTRACT:
				return GL_FUNC_REVERSE_SUBTRACT;
			case BlendOperation::MIN:
				return GL_MIN;
			case BlendOperation::MAX:
				return GL_MAX;

			default:
			{
				TUR_LOG_ERROR("Invalid or unsupported blending operation. Using default: ADD");
				return GL_FUNC_ADD;
			} break;
		}
	}

	static constexpr uint32_t GetLogicOp(LogicOperation logicOp)
	{
		switch (logicOp)
		{
			case LogicOperation::CLEAR:
				return GL_CLEAR;

			case LogicOperation::AND:
				return GL_AND;

			case LogicOperation::AND_REVERSE:
				return GL_AND_REVERSE;

			case LogicOperation::COPY:
				return GL_COPY;

			case LogicOperation::AND_INVERTED:
				return GL_AND_INVERTED;

			case LogicOperation::NO_OP:
				return GL_NOOP;

			case LogicOperation::XOR:
				return GL_XOR;

			case LogicOperation::OR:
				return GL_OR;

			case LogicOperation::NOR:
				return GL_NOR;

			case LogicOperation::EQUIVALENT:
				return GL_EQUIV;

			case LogicOperation::INVERT:
				return GL_INVERT;

			case LogicOperation::OR_REVERSE:
				return GL_OR_REVERSE;

			case LogicOperation::COPY_INVERTED:
				return GL_COPY_INVERTED;

			case LogicOperation::OR_INVERTED:
				return GL_OR_INVERTED;

			case LogicOperation::NAND:
				return GL_NAND;

			case LogicOperation::SET:
				return GL_SET;

			default:
			{
				TUR_LOG_ERROR("Invalid Color Blending Operation. Using default: COPY");
				return GL_COPY;
			} break;
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
    }

	void PipelineOpenGL::SetupShaders(const PipelineDescriptor& descriptor)
	{
		// TODO: Allow compute shaders.

		ShaderOpenGL* vertexShader = static_cast<ShaderOpenGL*>(descriptor.vertexShader);
		ShaderOpenGL* tessControlShader = static_cast<ShaderOpenGL*>(descriptor.tesselationControlShader);
		ShaderOpenGL* tessEvalShader = static_cast<ShaderOpenGL*>(descriptor.tesselationEvaluationShader);
		ShaderOpenGL* geometryShader = static_cast<ShaderOpenGL*>(descriptor.geometryShader);
		ShaderOpenGL* fragmentShader = static_cast<ShaderOpenGL*>(descriptor.fragmentShader);

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

