
project "ScriptTest"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",

		"SCRIPTTEST_EXPORTS"
	}

	files
	{
		"**.h",
		"**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Hudi/vendor/spdlog/include",
		"%{wks.location}/Hudi/vendor/glm",

		"src",
		"%{wks.location}/ECS/src",
		"%{wks.location}/HudiAPI/src",
		"%{wks.location}/Hudi/src"
	}

	links
	{
		"Hudi",
		"HudiAPI"
	}

	postbuildcommands
	{
		"{COPY} ../bin/" .. outputdir .. "/%{prj.name}/ScriptTest.dll ../HudiGen/assets/dlls/"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HD_PLATFORM_WINDOWS"
		}
		
	filter "system:not windows"
		defines
		{
			"HD_PLATFORM_MACOS"
		}


	filter "configurations:Debug"
		defines "HD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HD_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HD_DIST"
		runtime "Release"
		optimize "on"