local INCLUDE_DEPS = "include/deps/"
local OUT_DIR = "bin/Win64/"

-- Clean and copy Shared Libraries
os.rmdir("bin")
os.rmdir("build")

printf("Out dir: %s", OUT_DIR)
os.mkdir(OUT_DIR)

--GENERATE SOLUTION
solution "_SFML_IMGUI"
  defines {
    "_CRT_SECURE_NO_WARNINGS",
    "WIN32_LEAN_AND_MEAN",
    "NOMINMAX",
    "WIN64"
  }
  configuration{}
  windowstargetplatformversion "10.0.18362.0"   
  location "build"
  configurations {"Debug", "Release"}
    configuration "Debug"
      objdir "build/debug/obj"
      flags {
        "Symbols"
      }
      defines {
        "DEBUG"
      }
      targetsuffix "_d"
    configuration "Release"
      objdir "build/release/obj"
      flags {
        "OptimizeSpeed"
      }
    configuration{}
  platforms {"x64"}
  targetdir "bin/Win64/"
  debugdir "bin/Win64/"
  libdirs "bin/Win64/"
  targetextension ".exe"
      
--END GENERATE SOLUTION

--SFMLGui
project "SFMLGui"
  language "C++"
  kind "ConsoleApp"
  flags {
    "Cpp17"
  }
  --This causes D9025 warning
  buildoptions { "/Fddebug/SFMLGui.pdb" }
  includedirs {
    "include/",
    "include/deps/",
    INCLUDE_DEPS .. "imgui",
    INCLUDE_DEPS .. "nfd",
  }
  defines {
    "SFML_STATIC"
  }
  vpaths {
    ["Headers/*"] = "include/**.h",
    ["Sources/*"] = "src/**.cpp",
    ["Dependencies/*"] = INCLUDE_DEPS .. "**.*"
  }
  files {
    "include/**.h",
    "src/main.cpp",
    INCLUDE_DEPS .. "imgui/*.h",
    INCLUDE_DEPS .. "imgui/*.cpp",
    INCLUDE_DEPS .. "SFML/**.hpp",
    INCLUDE_DEPS .. "nfd/*.h",
    INCLUDE_DEPS .. "nfd/nfd_common.c",
    INCLUDE_DEPS .. "nfd/nfd_win.cpp",
  }
  libdirs {
    "deps/sfml/"
  }
  links {
    "opengl32",
    "freetype",
    "winmm",
    "gdi32",
    "openal32",
    "flac",
    "vorbisenc",
    "vorbisfile",
    "vorbis",
    "ogg",
    "comctl32"
  }
  configurations {"Debug", "Release"}
    configuration "Debug"
      flags {
        "ExtraWarnings"
      }
      links {
        "sfml-system-s-d",
        "sfml-window-s-d",
        "sfml-graphics-s-d",
        "sfml-window-s-d",
        "sfml-audio-s-d"
      }
    configuration "Release"
      links {
        "sfml-system-s",
        "sfml-window-s",
        "sfml-graphics-s",
        "sfml-window-s",
        "sfml-audio-s"
      }
    configuration{}
  
