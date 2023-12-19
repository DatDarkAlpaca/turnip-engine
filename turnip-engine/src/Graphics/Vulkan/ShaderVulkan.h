#pragma once
#include "Graphics/Shader.h"

namespace tur
{
	class ShaderVulkan : public Shader
	{
	public:
		explicit ShaderVulkan(const vk::Device& device, const ShaderDescriptor& descriptor);

	public:
		vk::ShaderModule GetModule() const { return m_ShaderModule; }

	private:
		vk::ShaderModule m_ShaderModule;
	};
}