#include "pch.hpp"
#include "opengl_device.hpp"
#include "opengl_gui_system.hpp"
#include "platform/platform.hpp"
#include "opengl_command_buffer.hpp"

namespace tur::gl
{
	OpenGLGUI::OpenGLGUI(GraphicsDeviceGL* device)
		: r_GraphicsDevice(device)
	{
		m_CommandBuffer = tur::make_unique<CommandBufferGL>(device->create_command_buffer());
		m_CommandBuffer->initialize();
	}

	void OpenGLGUI::initialize_impl()
	{
		initialize_opengl_gui(r_GraphicsDevice->r_Window);
	}
	void OpenGLGUI::shutdown_impl()
	{
		/* Blank */
	}

	void OpenGLGUI::set_clear_color_impl(const Color& color)
	{
		m_ClearColor = color;
	}

	void OpenGLGUI::begin_frame_impl()
	{
		begin_opengl_frame();
	}
	void OpenGLGUI::render_impl()
	{
		render_opengl_frame();
	}
	void OpenGLGUI::end_frame_impl()
	{
		end_opengl_frame();
	}

	void OpenGLGUI::add_texture_impl(texture_handle textureHandle)
	{
	}
	void OpenGLGUI::remove_texture_impl(texture_handle textureHandle)
	{
	}

	bool OpenGLGUI::texture_button_impl(texture_handle textureHandle, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int framePadding, const ImVec4& bgColor, const ImVec4& tintColor)
	{
		return ImGui::ImageButton((void*)r_GraphicsDevice->get_textures().get(textureHandle).handle, size, uv0, uv1, framePadding, bgColor, tintColor);
	}
	void OpenGLGUI::texture_impl(texture_handle textureHandle, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tintColor, const ImVec4& borderColor)
	{
		ImGui::Image((void*)r_GraphicsDevice->get_textures().get(textureHandle).handle, size, uv0, uv1, tintColor, borderColor);
	}
}