create_project("turnip-editor", "ConsoleApp")
links { "turnip-engine" }
includedirs { 
	"%{wks.location}/turnip-engine/",
    "%{wks.location}/turnip-engine/src"
}

filter "system:windows"
    postbuildcommands {
        "{COPYFILE} %[%{binaries_path}/turnip-script/turnip-script.dll] %[%{prj.location}]" 
    }
    
filter { }

