set_project("test")

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
    -- enable program debug features
    add_defines("VIXEL_DEBUG_ENABLED")
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

add_repositories(
    "vixel-local /home/brickmaster/projects/ViXeL/xmake-transition/ViXeL/build", 
    "aii-local /home/brickmaster/projects/antiintersectionism/antiintersectionism/build"
)
add_requires("vixel", {configs = {mode = "debug"}}, {alias = "ViXeL"})
add_requires("antiintersectionism", {configs = {mode = "debug"}}, {alias = "aii"})

add_requires("imgui", {configs = {sdl2 = true, opengl3 = true}})

target("test")
    set_kind("binary")
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    add_packages(
        "ViXeL",
        "imgui",
        "aii"
    )
    local output_dir = path.join("$(buildir)", "$(os)", "$(arch)", "$(mode)")
    after_build(function (target)
        os.cp("$(projectdir)/test/resources", target:targetdir())
    end)
    add_ldflags("-static", "-static-libgcc", "-static-libstdc++", {force = true})
    add_cxxflags("-static", "-static-libgcc", "-static-libstdc++", {force = true})
target_end()