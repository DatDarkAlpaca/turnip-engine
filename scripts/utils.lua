engineProjBase = _SCRIPT_DIR

function EngineIncludeDirs()
    local incDirs = { }

    for i, v in ipairs(incDirs) do
        incDirs[i] = path.rebase(v, engineProjBase, _SCRIPT_DIR)
    end
    
    includedirs { incDirs }
end