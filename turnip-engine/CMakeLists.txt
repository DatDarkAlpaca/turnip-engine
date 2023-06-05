cmake_minimum_required(VERSION 3.12)

find_package(spdlog CONFIG REQUIRED)

set(ENGINE_SOURCES
	"src/Turnip/Common/CPU_Information.cpp"
	"src/Turnip/Common/CPU_Information.h"
	"src/Turnip/Common/Assertion.h"
	"src/Turnip/Common/Debug_Definition.h"
	"src/Turnip/Common/DLL_Definition.h"
	"src/Turnip/Common/PlatformDetection.h"
	"src/Turnip/Common/Types.h"
	"src/pch.h"
	"src/test.cpp"
)

add_executable (turnip ${ENGINE_SOURCES})

target_link_libraries(turnip PRIVATE spdlog::spdlog spdlog::spdlog_header_only)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET turnip PROPERTY CXX_STANDARD 17)
endif()

target_include_directories(turnip PUBLIC
	"src/"
	"src/Turnip"
	"src/Turnip/Common"
)
