project "imgui"
	kind "StaticLib"
    warnings "off"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    systemversion "latest"

	targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

    files {
		"imgui/imconfig.h",
		"imgui/imgui.h",
		"imgui/imgui.cpp",
		"imgui/imgui_draw.cpp",
		"imgui/imgui_tables.cpp",
		"imgui/imgui_internal.h",
		"imgui/imgui_widgets.cpp",
		"imgui/imstb_rectpack.h",
		"imgui/imstb_textedit.h",
		"imgui/imstb_truetype.h",
		"imgui/imgui_demo.cpp"
	}

	filter "system:linux"
		pic "On"
    filter { }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
    filter {}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
        inlining "auto"
    filter { }