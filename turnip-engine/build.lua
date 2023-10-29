project "turnip-engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    files {
        "src/**.cpp",
        "src/**.h"
    }
   
    defines {
        "GLFW_STATIC"
    }

    includedirs {
        "%{prj.location}",
        "%{prj.location}/src"
    }

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
