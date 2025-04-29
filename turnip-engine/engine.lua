create_project("turnip-engine", "StaticLib")

mono_sdk_sgen_file = path.join(os.getenv("MONO_SDK"), "bin", "mono-2.0-sgen.dll")

links { "turnip-script" }

filter "system:windows"
    postbuildcommands {
        "{COPYFILE} %[%{mono_sdk_sgen_file}] %[%{!wks.location}bin/mono-2.0-sgen.dll]",
        "{COPYFILE} %[%{mono_sdk_sgen_file}] %[%{!wks.location}/mono-2.0-sgen.dll]"
    }
filter {}

postbuildcommands {
    "{COPYFILE} %[%{!binaries_path}/turnip-script/turnip-script.dll] %[%{!wks.location}bin/turnip-script.dll]",
}