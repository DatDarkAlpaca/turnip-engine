#include "pch.hpp"
#include <ImGuizmo.h>
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "platform/glfw/window_glfw.hpp"
#include "graphics/graphics.hpp"

static void OpenGLCallback(unsigned, unsigned type, unsigned, unsigned severity, int, const char* message, const void*)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:
			TUR_LOG_CRITICAL(message);
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			TUR_LOG_ERROR(message);
			break;

		case GL_DEBUG_SEVERITY_LOW:
			TUR_LOG_WARN(message);
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			TUR_LOG_INFO(message);
			break;
	}

	TUR_LOG_INFO("[OpenGL][Unknown severity]: {}", message);
}

namespace tur::gl
{
	void initialize_opengl_windowing(Window* window, const WindowProperties& properties, const GraphicsSpecification& specification)
	{
#ifdef TUR_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

#ifdef TUR_PLATFORM_MACOSX
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, specification.major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, specification.minor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
		glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		initialize_window(window, properties);

		glfwMakeContextCurrent(window->window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			TUR_LOG_CRITICAL("Failed to load OpenGL functions.");

#ifdef TUR_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		if (specification.major >= 4 && specification.minor >= 3)
		{
			glDebugMessageCallback(OpenGLCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		}
#endif
	}
	void initialize_opengl_gui(Window* window)
	{
		ImGui_ImplGlfw_InitForOpenGL(window->window, true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	void begin_opengl_frame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuizmo::BeginFrame();
	}
	void render_opengl_frame()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void end_opengl_frame()
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void present_opengl_window(Window* window)
	{
		glfwSwapBuffers(window->window);
	}
}