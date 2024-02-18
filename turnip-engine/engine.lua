project "turnip-engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    functionlevellinking "on"

    links {
        "imgui"
    }

    files {
        "src/Core/**.cpp",
        "src/Core/**.h",
        "src/Rendering/**.cpp",
        "src/Rendering/**.h",
        "src/Graphics/**.cpp",
        "src/Graphics/**.h",
        "src/Platform/OpenGL/**.h",
        "src/Platform/OpenGL/**.cpp",
        "src/Platform/Vulkan/**.h",
        "src/Platform/Vulkan/**.cpp",
        "src/Util/**.cpp",
        "src/Util/**.h",
        "src/Platform/*.h",
        "src/*.h",
        "src/*.cpp",
    }
   
    defines {
        "GLFW_STATIC",
        "GLFW_VULKAN_STATIC",
    }

    includedirs {
        "%{prj.location}",
        "%{prj.location}/src",
        "%{vendor_path}/imgui",
        "%{vendor_path}/imgui/backends",
    }

    -- Platform 
    DetectPlatform()

    -- Configurations
    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "on"
        defines { "TUR_DEBUG" }
    filter { }
        
    filter { "configurations:Release" }
        runtime "Release"
        optimize "on"
        inlining "auto"
    filter { }

    conan_setup()
