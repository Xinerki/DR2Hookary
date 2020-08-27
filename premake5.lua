systemversion "10.0.18362.0"
toolset "v142"
platforms { "x86" }

workspace "KG.DR2"
    configurations { "Debug", "Release" }

    location "build"

    symbols "On"
    staticruntime "On"

    filter "configurations:Debug"
        targetdir "bin/debug"
        defines { "DEBUG" }

    filter "configurations:Release"
        targetdir "bin/release"
        defines { "NDEBUG" }
        optimize "Size"

    filter {}

    flags { "NoIncrementalLink" }

    editandcontinue "Off"

group "vendor"

project "minhook"
    targetname "MinHook"
    language "C"
    kind "StaticLib"
    staticruntime "On"
    buildoptions "/MT"

    defines { "_ITERATOR_DEBUG_LEVEL=0" }

    files
    {
        "vendor/minhook/src/buffer.c",
        "vendor/minhook/src/hook.c",
        "vendor/minhook/src/trampoline.c",
        "vendor/minhook/src/hde/hde32.c"
    }

group "code"

project "mod"
    buildoptions '/std:c++latest'
    kind "SharedLib"
    language "C++"
    targetname "DR2Hookary"
    targetextension ".asi"

    buildoptions "/MT"

    staticruntime "On"

    pchheader "StdInc.h"
    pchsource "src/hook/StdInc.cpp"

    optimize "Off"
    characterset "MBCS"

    linkoptions "/OPT:REF"

    includedirs
    {
        "src/hook/",

        "vendor/minhook/include/",
        "vendor/injector/include/",
    }

    links
    {
        "minhook",

        "Wtsapi32",
    }

    defines { "_HAS_AUTO_PTR_ETC=1", "_ITERATOR_DEBUG_LEVEL=0" }

    files
    {
        "deps/cpp/**.cpp",
        "src/hook/**.cpp",
        "src/hook/**.h",
    }