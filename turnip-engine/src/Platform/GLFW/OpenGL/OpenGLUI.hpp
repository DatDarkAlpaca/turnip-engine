#pragma once
#include "Graphics/UI/UIBackend.hpp"

#include <glad/glad.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace tur
{
	class OpenGLUI : public UIBackend
	{
	public:
		void Initialize(platform::Window& window) override;

	public:
		void StartFrame() override;

		void EndFrame() override;

	private:
		static inline constexpr const char* g_OpenGLVersion = "#version 450";
		platform::Window* r_Window = nullptr;
	};
}