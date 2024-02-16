#pragma once
#include <vulkan/vulkan.hpp>
#include "Graphics/Shader.h"
#include "Util/File.h"

namespace tur::vulkan
{
	class Shader : public IShader
	{
	public:
		explicit Shader(const vk::Device& device, const ShaderDescriptor& descriptor)
		{
			auto sourceCode = ReadBinaryFile(descriptor.filepath);

			vk::ShaderModuleCreateInfo moduleInfo = {};
			moduleInfo.flags = vk::ShaderModuleCreateFlags();
			moduleInfo.codeSize = sourceCode.size();
			moduleInfo.pCode = reinterpret_cast<const uint32_t*>(sourceCode.data());

			try
			{
				m_ShaderModule = device.createShaderModule(moduleInfo);
			}
			catch (vk::SystemError err)
			{
				TUR_LOG_ERROR("Failed to create shader module from filepath: {}. {}", descriptor.filepath, err.what());
			}
		}

	public:
		vk::ShaderModule GetModule() const { return m_ShaderModule; }

	private:
		vk::ShaderModule m_ShaderModule;
	};
}