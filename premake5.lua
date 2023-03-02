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

include "Hudi/vendor/Glad"
include "Hudi/vendor/ImGui"

include "CodeGenerator"
include "ECS"
include "Hudi"
include "Sandbox"