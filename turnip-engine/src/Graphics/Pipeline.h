#pragma once
#include "Types/PrimitiveTopology.h"
#include "Types/Viewport.h"
#include "Types/Scissor.h"
#include "Types/PolygonMode.h"
#include "Types/CullMode.h"
#include "Types/FrontFace.h"
#include "Types/ColorBlending.h"

namespace tur
{
	struct PipelineDescriptor
	{
		// Vertex Input:
		uint32_t vertexBindingDescriptionCount = 0;
		uint32_t vertexAttributeDescriptionCount = 0;

		// Input Assembly:
		PrimitiveTopology topology = PrimitiveTopology::TRIANGLES;

		// Shaders:
		Shader* vertexShader = nullptr;
		Shader* tesselationControlShader = nullptr;
		Shader* tesselationEvaluationShader = nullptr;
		Shader* geometryShader = nullptr;
		Shader* fragmentShader = nullptr;

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

		// Layout:
		uint32_t layoutCount = 0;
		uint32_t pushConstantCount = 0;
	};

	class Pipeline
	{
	public:
		virtual ~Pipeline() = default;
	};
}