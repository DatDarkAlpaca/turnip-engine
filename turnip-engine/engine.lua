project "turnip-engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")
	
    pchheader "pch.hpp"
    pchsource "src/pch.cpp"
    
    files {
        "src/Core/**.cpp",
        "src/Core/**.hpp",
		"src/Core/**.c",
		
        "src/Rendering/**.cpp",
        "src/Rendering/**.hpp",
		"src/Rendering/**.c",
		
        "src/Graphics/**.cpp",
        "src/Graphics/**.hpp",
        "src/Graphics/**.c",
		
        "src/Util/**.cpp",
		"src/Util/**.hpp",
		"src/Util/**.c",
		
        "src/Platform/*.hpp",

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
