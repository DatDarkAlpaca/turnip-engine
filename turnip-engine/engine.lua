project "turnip-engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")
	
    pchheader "pch.h"
    pchsource "src/pch.cpp"

    libdirs {
        os.getenv("VULKAN_SDK") .. '/Lib/'
    }

    links {
        "imgui",
        "vulkan-1.lib"
    }
    
    files {
        "src/Core/**.cpp",
        "src/Core/**.h",
        "src/Rendering/**.cpp",
        "src/Rendering/**.h",
        "src/Graphics/**.cpp",
        "src/Graphics/**.h",
        "src/Util/**.cpp",
        "src/Util/**.h",
        "src/Platform/*.h",
        "src/*.h",
        "src/*.cpp",
    }
   
    includedirs {
        "%{prj.location}",
        "%{prj.location}/src",
    }

    -- Dependencies:
    setup_vendors()

    -- Platform 
    detect_platform()

    -- Configurations
    filter "system:windows"
        systemversion "latest"
    filter { }

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
