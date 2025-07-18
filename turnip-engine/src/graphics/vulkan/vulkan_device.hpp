 #pragma once
#include "core/free_list.hpp"
#include "vulkan_deletion_queue.hpp"
#include "graphics/graphics_device.hpp"

#include "objects/buffer.hpp"
#include "objects/texture.hpp"
#include "objects/pipeline.hpp"
#include "objects/descriptor.hpp"
#include "objects/vulkan_state.hpp"
#include "objects/render_target.hpp"

namespace tur::vulkan
{
	class CommandBufferVulkan;
	class VulkanGUI;

	class GraphicsDeviceVulkan : public BaseGraphicsDevice<GraphicsDeviceVulkan>
	{
		friend class BaseGraphicsDevice<GraphicsDeviceVulkan>;
		friend class CommandBufferVulkan;
		friend class VulkanGUI;
		friend void deletion::flush(deletion::DeletionQueue& deletionQueue);

		friend vk::DescriptorPool initialize_vulkan_gui(GraphicsDeviceVulkan*);
		friend Pipeline build_graphics_pipeline(GraphicsDeviceVulkan&, const PipelineDescriptor&);
		friend Texture build_texture(GraphicsDeviceVulkan* device, const TextureDescriptor& descriptor, const TextureAsset& asset);

	protected:
		void initialize_impl(NON_OWNING Window* window, const ConfigData& configData);
		void shutdown_impl();

		void begin_impl();
		void submit_impl();
		void present_impl();
		void end_impl();

	protected:
		CommandBufferVulkan create_command_buffer_impl();
		VulkanGUI create_gui_system_impl();
	
	protected:
		shader_handle create_shader_impl(const ShaderDescriptor& descriptor);
		void destroy_shader_impl(shader_handle textureHandle);

	protected:
		descriptor_handle create_descriptors_impl(const DescriptorSetLayoutDescriptor& descriptor);
		descriptor_set_handle create_descriptor_set_impl(descriptor_handle textureHandle);
		void update_descriptor_resource_impl(descriptor_set_handle descriptorSetHandle, handle_type resourceHandle, DescriptorType type, u32 binding);
		
	protected:
		pipeline_handle create_graphics_pipeline_impl(const PipelineDescriptor& descriptor);

	protected:
		buffer_handle create_default_buffer_impl(const BufferDescriptor& descriptor, const DataBuffer& data);
		buffer_handle create_buffer_impl(const BufferDescriptor& descriptor, u32 size);
		void update_buffer_impl(buffer_handle textureHandle, const DataBuffer& data, u32 offset);
		void* map_buffer_impl(buffer_handle textureHandle, u32 offset, u32 length, AccessFlags flags);
		void copy_buffer_impl(buffer_handle source, buffer_handle destination, u32 size, u32 srcOffset, u32 dstOffset);
		void copy_buffer_to_texture_impl(buffer_handle source, texture_handle destination, u32 width, u32 height);
		void destroy_buffer_impl(buffer_handle &textureHandle);

	protected:
		texture_handle create_texture_impl(const TextureDescriptor& descriptor, const TextureAsset& asset);
		texture_handle create_texture_impl(const TextureDescriptor& descriptor);
		void update_texture_impl(texture_handle textureHandle, const TextureAsset& asset);
		void destroy_texture_impl(texture_handle& textureHandle);

	protected:
		render_target_handle create_render_target_impl(const RenderTargetDescriptor& descriptor);
		void resize_render_target_impl(render_target_handle& textureHandle, u32 width, u32 height);
		void destroy_render_target_impl(render_target_handle& textureHandle);

	protected:
		inline Texture get_native_texture_impl(texture_handle textureHandle) { return m_Textures.get(textureHandle); }

	public:
		void wait_idle();
		void submit_immediate_command(std::function<void()>&& function);
		void transition_texture_layout(Texture& texture, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
		void transition_texture_layout(texture_handle textureHandle, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
		void copy_buffer_to_texture_direct(Buffer& buffer, Texture& texture, u32 width, u32 height);

	public:
		inline void add_frame_begin_work(std::function<void()>&& work)
		{
			m_FrameStartWorkQueue.push_back(std::move(work));
		}

	private:
		void recreate_swapchain();

	private:
		inline free_list<vk::ShaderModule>& get_shader_modules() { return m_ShaderModules; }
		inline free_list<Descriptor>& get_descriptors() { return m_Descriptors; }
		inline free_list<DescriptorSet>& get_descriptor_sets() { return m_DescriptorSets; }
		inline free_list<Pipeline>& get_pipelines() { return m_Pipelines; }
		inline free_list<Texture>& get_textures() { return m_Textures; }
		inline free_list<Buffer>& get_buffers() { return m_Buffers; }
		inline free_list<RenderTarget>& get_render_targets() { return m_RenderTargets; }

		inline VulkanState& get_state() { return m_State; }
		inline vk::CommandBuffer get_imm_command_buffer() const { return m_ImmCommandBuffer; }
		inline Window* get_window() { return r_Window; }

	private:
		Window* r_Window = nullptr;
		VulkanState m_State;
		ConfigData m_ConfigData;

	private:
		free_list<vk::ShaderModule> m_ShaderModules;
		free_list<Descriptor> m_Descriptors;
		free_list<DescriptorSet> m_DescriptorSets;

		free_list<Pipeline> m_Pipelines;
		free_list<Texture> m_Textures;
		free_list<Buffer> m_Buffers;
		free_list<RenderTarget> m_RenderTargets;

	private:
		deletion::DeletionQueue m_DeletionQueue;

	private:
		vk::Fence m_ImmFence;
		vk::CommandPool m_ImmCommandPool;
		vk::CommandBuffer m_ImmCommandBuffer;

	private:
		std::vector<std::function<void()>> m_FrameStartWorkQueue;
	};
}