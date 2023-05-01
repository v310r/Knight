workspace "Knight"
    startproject "Knight"
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Knight"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "Off"

    targetdir ("build/bin/" .. outputdir)
    objdir ("build/intermediate/" .. outputdir)

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "extern/SFML-2.5.1/include"
    }

    libdirs
    {
        "extern/SFML-2.5.1/lib"
    }

    links
    {
        "opengl32.lib",
        "winmm.lib",
        "gdi32.lib",
        "freetype.lib",
        "shlwapi.lib"
    }

    filter "system:windows"
        systemversion = "latest"

        defines
        {
            "_CONSOLE",
            "SFML_STATIC"
        }
    
    filter "configurations:Debug"
        symbols "On"
        defines
        {
            "_DEBUG"
        }

        links
        {
            "sfml-window-s-d.lib",
            "sfml-graphics-s-d.lib",
            "sfml-system-s-d.lib"
        }
    
    filter "configurations:Release"
        optimize "On"
        defines
        {
            "NDEBUG"
        }

        links
        {
            "sfml-window-s.lib",
            "sfml-graphics-s.lib",
            "sfml-system-s.lib"
        }
