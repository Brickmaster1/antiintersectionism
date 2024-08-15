set_project("antiintersectionism")

set_languages("c++20")

add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")
is_windows = is_plat("windows") or is_plat("mingw")

-- the debug mode
if is_mode("debug") then
    -- enable the debug symbols
    set_symbols("debug")
    -- disable optimization
    set_optimize("none")
end
-- the release mode
if is_mode("release") then
    -- set the symbols visibility: hidden
    set_symbols("hidden")
    -- enable fastest optimization
    set_optimize("fastest")
    -- strip all symbols
    set_strip("all")
end

function add_headerfiles_subdir(pattern, options)
    local prefixdir = options.prefixdir or ""
    local rootdir = options.rootdir or "include"

    local headerfiles = os.files(rootdir .. pattern)

    for _, headerfile in ipairs(headerfiles) do
        local dir = path.directory(headerfile)
        
        if dir == rootdir then
            add_headerfiles(headerfile, {prefixdir = prefixdir})
        else
            local fileprefixdir = dir:gsub("^" .. rootdir .. "/", "")
            add_headerfiles(headerfile, {prefixdir = path.join(prefixdir, fileprefixdir)})
        end
    end
end

add_requires(
    "glm"
)

target("antiintersectionism")
    set_kind("static")
    add_files("src/**.cpp")
    add_headerfiles_subdir("**.h", {rootdir = "src", prefixdir = "aii"})
    add_packages(
        "glm"
    )
    add_ldflags("-static", "-static-libgcc", "-static-libstdc++", {force = true})
    add_cxxflags("-static", "-static-libgcc", "-static-libstdc++", {force = true})
target_end()