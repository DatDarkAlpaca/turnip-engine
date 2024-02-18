#pragma once
#include "Common.h"
#include <glad/glad.h>

namespace tur::gl
{
	inline void ShowOpenGLVersion()
	{
		TUR_LOG_INFO("OpenGL {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}
}