#pragma once
#include "Common.h"

#pragma warning(push)
#pragma warning(disable: 4005)
#pragma warning(disable: 6285)
#pragma warning(disable: 4267)
#pragma warning(disable: 4275)
#pragma warning(disable: 26800)
#pragma warning(disable: 26498)
#pragma warning(disable: 26827)
#pragma warning(disable: 26451)

#include <unordered_map>
#include <string_view>
#include <filesystem>
#include <functional>
#include <execution>
#include <numeric>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <memory>
#include <array>
#include <bitset>

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#ifdef TUR_WINDOWING_GLFW
	#define GLFW_INCLUDE_NONE
	#include <imgui_impl_glfw.h>
	#include <GLFW/glfw3.h>
	#include <glad/glad.h>
#endif

#ifdef TUR_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsHeaders.h"
	#include <intrin.h>
#endif

#include <imgui.h>
#include <imgui_impl_opengl3.h> // TODO: REQUIRES_GL(...)

#include <spdlog/spdlog.h>
#include <entt/entt.hpp>

#pragma warning(pop)