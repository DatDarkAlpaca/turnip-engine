newoption {
    description = "Uses the specified graphics api",

    trigger     = "graphics_api",
    value       = "API",
    default     = "opengl",
    category    = "Build Options",

    allowed = {
        { "opengl",    "OpenGL" },
        { "vulkan",    "Vulkan" }
    }
}

function define_graphics_api()
    if not _OPTIONS["graphics_api"] then
        defines { "TUR_API_VULKAN" }
        return
    end

    local api = _OPTIONS["graphics_api"]:lower()
    if api == "opengl" then
        
        defines { "TUR_API_OPENGL" }
    elseif api  == "vulkan" then
        defines { "TUR_API_VULKAN" }
    else
        error("Invalid graphics API specified. Use 'opengl' or 'vulkan'.")
    end
end