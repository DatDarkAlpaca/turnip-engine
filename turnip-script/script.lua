project "turnip-script"
    kind "SharedLib"
    language "C#"

    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

    files {
        "src/**.cs",
	}

    postbuildcommands {
        "{COPYFILE} %[%{!binaries_path}/turnip-script/turnip-script.dll] %[%{!wks.location}bin/turnip-script.dll]",
    }