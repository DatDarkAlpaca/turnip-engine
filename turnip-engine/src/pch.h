#pragma once
#pragma warning(push, 0)
#include "Common.h"

#ifdef TUR_PLATFORM_WINDOWS
	#include <intrin.h>
#else
	#include <signal.h>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>

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

#pragma warning(pop)