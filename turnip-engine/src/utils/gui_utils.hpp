#pragma once
#include <imgui.h>
#include <glm/glm.hpp>

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