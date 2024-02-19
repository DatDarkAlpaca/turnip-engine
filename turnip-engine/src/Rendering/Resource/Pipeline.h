#pragma once
#include "Common.h"
#include <vector>

#include "Rendering/Type/Types.h"
#include "InputLayout.h"
#include "Renderpass.h"
#include "Shader.h"

namespace tur
{
	struct PipelineStateDescriptor
	{
		uint32_t vertexBindingDescriptionCount = 0;
		uint32_t vertexAttributeDescriptionCount = 0;

		// Input Assembly:
		PrimitiveTopology primitiveTopology = PrimitiveTopology::TRIANGLES;

		// Shaders:
		ShaderHandle vertexShader = ShaderHandle::INVALID;
		ShaderHandle tesselationControlShader = ShaderHandle::INVALID;
		ShaderHandle tesselationEvaluationShader = ShaderHandle::INVALID;
		ShaderHandle geometryShader = ShaderHandle::INVALID;
		ShaderHandle fragmentShader = ShaderHandle::INVALID;

		// Viewports:
		std::vector<Viewport> viewports;

		// Scissor:
		std::vector<Scissor> scissors;

		// Rasterizer:
		bool clampDepth = false;
		bool discardFragments = false;
		bool enableDepthBias = false;
		PolygonMode polygonMode = PolygonMode::FILL;
		float lineWidth = 1.0f;
		CullMode cullMode = CullMode::BACK;
		FrontFace frontFace = FrontFace::CLOCKWISE;

		// Multisample:
		bool enableMultisampling = false;
		uint32_t sampleCount = 1;

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

		// Renderpass:
		RenderpassHandle renderpass = RenderpassHandle::INVALID;

		// Layout:
		uint32_t layoutCount = 0;
		uint32_t pushConstantCount = 0;
	};

	enum PipelineStateHandle : uint32_t { INVALID = InvalidHandle };
}