#pragma once
#include <vector>
#include "CommonGraphics.hpp"

#include "VertexFormat.hpp"
#include "Shader.hpp"


// TODO: implement missing parameters

namespace tur
{
	using pipeline_handle = uint32_t;

	struct PipelineDescriptor
	{
		// Vertex Format:
		VertexFormat vertexFormat;

		// Input Assembly:
		PrimitiveTopology primitiveTopology = PrimitiveTopology::TRIANGLES;

		// Shaders:
		shader_handle vertexShader = invalid_handle;
		shader_handle tesselationControlShader = invalid_handle;
		shader_handle tesselationEvaluationShader = invalid_handle;
		shader_handle geometryShader = invalid_handle;
		shader_handle fragmentShader = invalid_handle;

		// Viewports:
		// std::vector<Viewport> viewports;

		// Scissor:
		// std::vector<Scissor> scissors;
		
		// Rasterizer:
		bool clampDepth = false;
		bool discardFragments = false;
		bool enableDepthBias = false;
		PolygonMode polygonMode = PolygonMode::FILL;
		float lineWidth = 1.0f;
		CullMode cullMode = CullMode::FRONT;
		FrontFace frontFace = FrontFace::COUNTER_CLOCKWISE;

		// Multisample:
		// bool enableMultisampling = false;
		// uint32_t sampleCount = 1;

		// Color Blending:
		// ColorComponents blendingWriteMask = ColorComponents::R | ColorComponents::G | ColorComponents::B | ColorComponents::A;
		// bool enableColorBlending = false;
		// BlendFactor srcColorBlendFactor = BlendFactor::ONE;
		// BlendFactor dstColorBlendFactor = BlendFactor::ZERO;
		// BlendFactor srcAlphaColorBlendFactor = BlendFactor::ONE;
		// BlendFactor dstAlphaColorBlendFactor = BlendFactor::ZERO;
		// BlendOperation colorBlendOp = BlendOperation::ADD;
		// BlendOperation alphaBlendOp = BlendOperation::ADD;
		// bool enableLogicOp = false;
		// LogicOperation logicOperation = LogicOperation::COPY;
		// float blendConstants[4] = { 0.f, 0.f, 0.f, 0.f };
	};
}