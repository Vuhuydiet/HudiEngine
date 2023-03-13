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

group "Dependencies"
	include "Hudi/vendor/Glad"
	include "Hudi/vendor/ImGui"
	include "ECS"
group ""

group "Utilities"
	include "CodeGenerator"
group ""

group "Core"
	include "Hudi"
group ""

group "Test"
	include "Sandbox"
	include "ImGuiTest"
group ""