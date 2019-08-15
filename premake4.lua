local INCLUDE_DEPS = "include/deps/"
local OUT_DIR = "bin/Win64/"

os.rmdir("build")

printf("Out dir: %s", OUT_DIR)
os.mkdir(OUT_DIR)

--GENERATE SOLUTION
solution "_glsl-spv"
  defines {
    "_CRT_SECURE_NO_WARNINGS",
    "WIN32_LEAN_AND_MEAN",
    "NOMINMAX",
    "WIN64"
  }
  configuration{}
  windowstargetplatformversion "10.0.18362.0"   -- Change this to your current one 
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
        "OptimizeSize"
      }
    configuration{}
  platforms {"x64"}
  targetdir "bin/Win64/"
  debugdir "bin/Win64/"
  libdirs "bin/Win64/"
  targetextension ".exe"
      
--END GENERATE SOLUTION

--glsl-spv
project "glsl-spv"
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
    INCLUDE_DEPS .. "INIReader",
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
    "include/*.h",
    "src/*.cpp",
    INCLUDE_DEPS .. "imgui/*.h",
    INCLUDE_DEPS .. "imgui/*.cpp",
    INCLUDE_DEPS .. "SFML/**.hpp",
    INCLUDE_DEPS .. "nfd/*.h",
    INCLUDE_DEPS .. "nfd/nfd_common.c",
    INCLUDE_DEPS .. "nfd/nfd_win.cpp",
    INCLUDE_DEPS .. "INIReader/INIReader.h",
  }
  libdirs {
    "deps/sfml/"
  }
  links {
    "opengl32",
    "freetype",
    "winmm",
    "gdi32",
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
        "sfml-graphics-s-d"
      }
    configuration "Release"
      links {
        "sfml-system-s",
        "sfml-window-s",
        "sfml-graphics-s"
      }
    configuration{}
  
