#pragma once
#include <imgui.h>

#include "graphics/gui_system.hpp"
#include "opengl_command_buffer.hpp"

namespace tur::gl
{
	class GraphicsDeviceGL;

	class OpenGLGUI : public BaseGUISystem<OpenGLGUI>
	{
		friend class BaseGUISystem<OpenGLGUI>;

	public:
		OpenGLGUI(GraphicsDeviceGL* device);

	protected:
		void initialize_impl();
		void shutdown_impl();

	protected:
		void set_clear_color_impl(const Color& color);

	protected:
		void begin_frame_impl();
		void render_impl();
		void end_frame_impl();

	protected:
		void add_texture_impl(texture_handle textureHandle);
		void remove_texture_impl(texture_handle textureHandle);

		bool texture_button_impl(texture_handle textureHandle, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int framePadding, const ImVec4& bgColor, const ImVec4& tintColor);
		void texture_impl(texture_handle textureHandle, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tintColor, const ImVec4& borderColor);

	private:
		NON_OWNING GraphicsDeviceGL* r_GraphicsDevice = nullptr;
		tur_unique<CommandBufferGL> m_CommandBuffer;

	private:
		Color m_ClearColor;
	};
}