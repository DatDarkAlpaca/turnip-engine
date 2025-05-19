premake.modules.clear = {}
local m = premake.modules.clear
local p = premake

newaction {
    trigger = "clear",
    description = "Clears generated project files and binaries",

    execute = function()
        os.rmdir("./build")
        os.rmdir("./bin")
        os.rmdir("./intermediate")
        os.rmdir("./.vs")
		
        os.remove('**.sln')
        os.remove('**.vcxproj')
        os.remove('**.filters')
        os.remove("**.user")
        os.remove("**.csproj")
        os.remove("**.dll")        
        os.remove('**.props')
        os.remove('**.targets')
        os.remove('**.Debug')
        os.remove('**.Release')
        os.remove('**.log')
        os.remove('**.ini')
        os.remove("Makefile")
		os.remove("**.make")
    end
}

return m
