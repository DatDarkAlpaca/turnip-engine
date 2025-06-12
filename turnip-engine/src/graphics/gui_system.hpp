#pragma once
#include "graphics/objects/texture.hpp"
#include "utils/color.hpp"

namespace tur
{
	template<typename Backend>
	class BaseGUISystem
	{
	public:
		void initialize()
		{
			static_cast<Backend*>(this)->initialize_impl();
		}
		void shutdown()
		{
			static_cast<Backend*>(this)->shutdown_impl();
		}

	public:
		void set_clear_color(const Color& color)
		{
			static_cast<Backend*>(this)->set_clear_color_impl(color);
		}

	public:
		void begin_frame()
		{
			static_cast<Backend*>(this)->begin_frame_impl();
		}
		void render()
		{
			static_cast<Backend*>(this)->render_impl();
		}
		void end_frame()
		{
			static_cast<Backend*>(this)->end_frame_impl();
		}

	public:
		void add_texture(texture_handle textureHandle)
		{
			static_cast<Backend*>(this)->add_texture_impl(textureHandle);
		}

		void remove_texture(texture_handle textureHandle)
		{
			static_cast<Backend*>(this)->remove_texture_impl(textureHandle);
		}

		bool texture_button(texture_handle textureHandle, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int framePadding = -1, const ImVec4& bgColor = ImVec4(0, 0, 0, 0), const ImVec4& tintColor = ImVec4(1, 1, 1, 1))
		{
			return static_cast<Backend*>(this)->texture_button_impl(textureHandle, size, uv0, uv1, framePadding, bgColor, tintColor);
		}

		void texture(texture_handle textureHandle, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tintColor = ImVec4(1, 1, 1, 1), const ImVec4& borderColor = ImVec4(0, 0, 0, 0))
		{
			static_cast<Backend*>(this)->texture_impl(textureHandle, size, uv0, uv1, tintColor, borderColor);
		}
	};
}