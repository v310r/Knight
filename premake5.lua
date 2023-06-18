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
            --"extern/SFML-2.5.1/include",
            "%{wks.location}/extern/SFML/include",
            "%{prj.location}/src"
        }

        libdirs
        {
            "extern/SFML/extlibs/bin/x64",
            "extern/SFML/extlibs/libs-msvc/x64"
        }

        links
        {
            "sfml-audio",
            "sfml-graphics",
            "sfml-network",
            "sfml-system",
            "sfml-window"
        }

        filter "system:windows"
            systemversion = "latest"

            defines
            {
                "_CONSOLE",
                --"SFML_STATIC"
            }
        
            links
            {
                "opengl32.lib",
                "winmm.lib",
                "gdi32.lib",
                "freetype.lib",
                "shlwapi.lib"
            }
            
            filter "configurations:Debug"
                symbols "On"
                defines
                {
                    "_DEBUG"
                }

                postbuildcommands 
                {
                    'IF NOT EXIST "%{cfg.targetdir}/sfml-graphic.pdb" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Debug/sfml-graphics.pdb" "%{cfg.targetdir}"', ')',
                    'IF NOT EXIST "%{cfg.targetdir}/sfml-graphics-d.exp" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Debug/sfml-graphics-d.exp" "%{cfg.targetdir}"', ')',
                    'IF NOT EXIST "%{cfg.targetdir}/sfml-graphics-d-2.dll" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Debug/sfml-graphics-d-2.dll" "%{cfg.targetdir}"', ')',

                    'IF NOT EXIST "%{cfg.targetdir}/sfml-audio.pdb" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Debug/sfml-audio.pdb" "%{cfg.targetdir}"', ')',
                    'IF NOT EXIST "%{cfg.targetdir}/sfml-audio-d.exp" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Debug/sfml-audio-d.exp" "%{cfg.targetdir}"', ')',
                    'IF NOT EXIST "%{cfg.targetdir}/sfml-audio-d-2.dll" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Debug/sfml-audio-d-2.dll" "%{cfg.targetdir}"', ')',

                    'IF NOT EXIST "%{cfg.targetdir}/sfml-network.pdb" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Debug/sfml-network.pdb" "%{cfg.targetdir}"', ')',
                    'IF NOT EXIST "%{cfg.targetdir}/sfml-network-d.exp" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Debug/sfml-network-d.exp" "%{cfg.targetdir}"', ')',
                    'IF NOT EXIST "%{cfg.targetdir}/sfml-network-d-2.dll" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Debug/sfml-network-d-2.dll" "%{cfg.targetdir}"', ')',

                    'IF NOT EXIST "%{cfg.targetdir}/sfml-system.pdb" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Debug/sfml-system.pdb" "%{cfg.targetdir}"', ')',
                    'IF NOT EXIST "%{cfg.targetdir}/sfml-system-d.exp" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Debug/sfml-system-d.exp" "%{cfg.targetdir}"', ')',
                    'IF NOT EXIST "%{cfg.targetdir}/sfml-system-d-2.dll" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Debug/sfml-system-d-2.dll" "%{cfg.targetdir}"', ')',

                    'IF NOT EXIST "%{cfg.targetdir}/sfml-window.pdb" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Debug/sfml-window.pdb" "%{cfg.targetdir}"', ')',
                    'IF NOT EXIST "%{cfg.targetdir}/sfml-window-d.exp" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Debug/sfml-window-d.exp" "%{cfg.targetdir}"', ')',
                    'IF NOT EXIST "%{cfg.targetdir}/sfml-window-d-2.dll" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Debug/sfml-window-d-2.dll" "%{cfg.targetdir}"', ')'
                }

                libdirs
                {
                    "extern/SFML/build/lib/Debug"
                }

                links
                {

                    --"sfml-window-s-d.lib",
                    --"sfml-graphics-s-d.lib",
                    --"sfml-system-s-d.lib"

                    --"sfml-audio-d.lib",
                    --"sfml-graphics-d.lib",
                    --"sfml-network-d.lib",
                    --"sfml-system-d.lib",
                    --"sfml-window-d.lib",

                    --"sfml-audio-d.lib",
                    --"sfml-graphics-d.lib",
                    --"sfml-network-d.lib",
                    --"sfml-system-d.lib",
                    --"sfml-window-d.lib"
                }
            
            filter "configurations:Release"
                optimize "On"
                defines
                {
                    "NDEBUG"
                }
                
                libdirs
                {
                    "extern/SFML/build/lib/Release"
                }

                postbuildcommands 
                {
                    'IF NOT EXIST "%{cfg.targetdir}/sfml-graphics-2.dll" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Release/sfml-graphics-2.dll" "%{cfg.targetdir}"', ')',

                    'IF NOT EXIST "%{cfg.targetdir}/sfml-audio-2.dll" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Release/sfml-audio-2.dll" "%{cfg.targetdir}"', ')',

                    'IF NOT EXIST "%{cfg.targetdir}/sfml-network-2.dll" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Release/sfml-network-2.dll" "%{cfg.targetdir}"', ')',

                    'IF NOT EXIST "%{cfg.targetdir}/sfml-system-2.dll" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Release/sfml-system-2.dll" "%{cfg.targetdir}"', ')',

                    'IF NOT EXIST "%{cfg.targetdir}/sfml-window-2.dll" (', '{COPY} "%{wks.location}/extern/SFML/build/lib/Release/sfml-window-2.dll" "%{cfg.targetdir}"', ')'
                }

                links
                {
                    --"sfml-audio.lib",
                    --"sfml-graphics.lib",
                    --"sfml-network.lib",
                    --"sfml-system.lib",
                    --"sfml-window.lib"
                }

                --links
                --{
                --    "sfml-window-s.lib",
                --    "sfml-graphics-s.lib",
                --    "sfml-system-s.lib"
                --}

    externalproject "sfml-audio"
        location "%{wks.location}/extern/SFML/build/src/SFML/Audio"
        kind "SharedLib"
        language "C++"

    externalproject "sfml-graphics"
        location "%{wks.location}/extern/SFML/build/src/SFML/Graphics"
        kind "SharedLib"
        language "C++"

    externalproject "sfml-network"
        location "%{wks.location}/extern/SFML/build/src/SFML/Network"
        kind "SharedLib"
        language "C++"

    externalproject "sfml-system"
        location "%{wks.location}/extern/SFML/build/src/SFML/System"
        kind "SharedLib"
        language "C++"

    externalproject "sfml-window"
        location "%{wks.location}/extern/SFML/build/src/SFML/Window"
        kind "SharedLib"
        language "C++"

