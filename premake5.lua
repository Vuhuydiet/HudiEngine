workspace "HudiEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "CodeGenerator/premake5.lua"
include "ECS/premake5.lua"
include "Hudi/premake5.lua"
include "Sandbox/premake5.lua"
include "PKGod/premake5.lua"