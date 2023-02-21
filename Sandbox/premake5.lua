
project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"src/Project",
		"%{wks.location}/ECS/src",
		"%{wks.location}/Hudi/src",

		"%{wks.location}/Hudi/vendor/spdlog/include",
		"%{wks.location}/Hudi/vendor/SDL2/include",
		"%{wks.location}/Hudi/vendor/SDL2_image/include"
	}

	links
	{
		"Hudi"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HD_PLATFORM_WINDOWS"
		}
		
		prebuildcommands
		{
			"{COPY} ../Dependencies/ ../bin/" .. outputdir .. "/%{prj.name}"
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