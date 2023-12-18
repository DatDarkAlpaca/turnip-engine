#pragma once
#include "Graphics/Shader.h"

namespace tur
{
	class ShaderVulkan : public Shader
	{
		friend class BackendVulkan;

	private:
		explicit ShaderVulkan(const vk::Device& device, const ShaderDescriptor& descriptor);

	private:
		vk::ShaderModule m_ShaderModule;
	};
}