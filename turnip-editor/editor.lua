create_project("turnip-editor", "ConsoleApp")
links { "turnip-engine" }
includedirs { 
	"%{wks.location}/turnip-engine/",
    "%{wks.location}/turnip-engine/src"
}