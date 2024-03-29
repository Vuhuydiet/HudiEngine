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
	include "Hudi/vendor/Box2D"
	include "ECS"
group ""

group "Core"
	include "Hudi"
	include "HudiGen"
	include "HudiAPI"
group ""

group "Test"
	include "ScriptTest"
group ""