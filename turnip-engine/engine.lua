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
        "src/Core/**.hpp",
		"src/Core/**.c",
        "src/Core/**.h",
		
        "src/Rendering/**.cpp",
        "src/Rendering/**.hpp",
		"src/Rendering/**.c",
        "src/Rendering/**.h",
		
        "src/Graphics/**.cpp",
        "src/Graphics/**.hpp",
        "src/Graphics/**.c",
        "src/Graphics/**.h",
		
        "src/Util/**.cpp",
		"src/Util/**.hpp",
        "src/Util/**.h",
		"src/Util/**.c",
		
        "src/Platform/*.hpp",
		"src/Platform/*.h",

		"src/*.cpp",
		"src/*.hpp",
        "src/*.c",
		"src/*.h",
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
