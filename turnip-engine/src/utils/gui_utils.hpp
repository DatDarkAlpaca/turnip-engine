#pragma once
#include <imgui.h>
#include <glm/glm.hpp>
#include "graphics/system.hpp"

namespace tur
{
	inline glm::uvec2 get_mouse_pixel_position()
	{
		ImVec2 mousePos = ImGui::GetMousePos();

		ImVec2 imageMin = ImGui::GetItemRectMin();
		ImVec2 imageMax = ImGui::GetItemRectMax();

		// Calculate normalized coordinates (0-1)
		float x = (mousePos.x - imageMin.x) / (imageMax.x - imageMin.x);
		float y = (mousePos.y - imageMin.y) / (imageMax.y - imageMin.y);

		// If your image has UV coordinates flipped (like your {0,1} to {1,0} suggests)
		y = 1.0f - y;

		return { x, y };
	}

	inline glm::uvec2 get_mouse_pixel_position(float width, float height)
	{
		auto size = get_mouse_pixel_position();
		return { size.x * width, size.y * height };
	}
}

namespace ImGui
{
	// TODO: make it cross-api
	inline bool TextureButton(GraphicsDevice* device, texture_handle handle, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int framePadding = -1, const ImVec4& bgColor = ImVec4(0, 0, 0, 0), const ImVec4& tintColor = ImVec4(1, 1, 1, 1))
	{
		
#ifdef TUR_API_OPENGL
		return ImGui::ImageButton((void*)device->get_native_texture(handle).handle, size, uv0, uv1, framePadding, bgColor, tintColor);
#else TUR_API_VULKAN
		return false;
#endif
	}

	inline void Texture(GraphicsDevice* device, texture_handle handle, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tintColor = ImVec4(1, 1, 1, 1), const ImVec4& borderColor = ImVec4(0, 0, 0, 0))
	{
#ifdef TUR_API_OPENGL
		ImGui::Image((void*)device->get_native_texture(handle).handle, size, uv0, uv1, tintColor, borderColor);
#else TUR_API_VULKAN
		ImGui::Button("hi");
#endif
	}
}