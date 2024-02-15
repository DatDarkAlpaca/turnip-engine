#pragma once
#include "Common.h"
#include <vector>

#include "Rendering/Type/Types.h"
#include "InputLayout.h"
#include "Shader.h"

namespace tur
{
	struct PipelineStateDescriptor
	{
		// Input Assembly:
		PrimitiveTopology primitiveTopology = PrimitiveTopology::TRIANGLES;

		// Shaders:
		ShaderHandle vertexShader = ShaderHandle::INVALID;
		ShaderHandle tesselationControlShader = ShaderHandle::INVALID;
		ShaderHandle tesselationEvaluationShader = ShaderHandle::INVALID;
		ShaderHandle geometryShader = ShaderHandle::INVALID;
		ShaderHandle fragmentShader = ShaderHandle::INVALID;

		// Rasterizer:
		PolygonMode polygonMode = PolygonMode::FILL;
		CullMode cullMode = CullMode::BACK;
		FrontFace frontFace = FrontFace::CLOCKWISE;

		// Layout:
		std::vector<InputLayoutElement> inputLayouts;

		// Color Blending:
		ColorComponents blendingWriteMask = ColorComponents::R | ColorComponents::G | ColorComponents::B | ColorComponents::A;
		bool enableColorBlending = false;
		BlendFactor srcColorBlendFactor = BlendFactor::ONE;
		BlendFactor dstColorBlendFactor = BlendFactor::ZERO;
		BlendFactor srcAlphaColorBlendFactor = BlendFactor::ONE;
		BlendFactor dstAlphaColorBlendFactor = BlendFactor::ZERO;
		BlendOperation colorBlendOp = BlendOperation::ADD;
		BlendOperation alphaBlendOp = BlendOperation::ADD;
		bool enableLogicOp = false;
		LogicOperation logicOperation = LogicOperation::COPY;
		float blendConstants[4] = { 0.f, 0.f, 0.f, 0.f };
	};

	enum PipelineStateHandle : uint32_t { INVALID = InvalidHandle };
}