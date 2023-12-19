#include "pch.h"
#include "ShaderVulkan.h"
#include "Util/File.h"

namespace tur
{
	ShaderVulkan::ShaderVulkan(const vk::Device& device, const ShaderDescriptor& descriptor)
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
}