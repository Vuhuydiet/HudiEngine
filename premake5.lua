workspace "HudiEngine"
	architecture "x64"
	startproject "HudiGen"

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
	include "Hudi/vendor/yaml-cpp"
	include "ECS"
group ""

group "Core"
	include "Hudi"
	include "HudiGen"
group ""

group "Test"
	include "Sandbox"
	include "ImGuiTest"
group ""