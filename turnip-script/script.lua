project "turnip-script"
    kind "SharedLib"
    language "C#"

    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

        
    files {
        "src/**.cs",
	}