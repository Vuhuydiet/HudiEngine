project "Hudi"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hdpch.h"
	pchsource "src/hdpch.cpp"

	files
    {
        "src/**.h",
        "src/**.cpp",
		"vendor/glm/**.hpp",
		"vendor/glm/**.inl"
    }

	includedirs
	{
		"vendor/spdlog/include",
		"vendor/SDL2/include",
		"vendor/SDL2_image/include",
		"vendor/Glad/include",
		"vendor/ImGui",
		"vendor/glm",

		"src",
		"%{wks.location}/ECS/src"
	}

	libdirs
	{
		"vendor/SDL2/lib/x64",
		"vendor/SDL2_image/lib/x64"
	}

	links
	{
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib",
		"opengl32.lib",

		"Glad",
		"ImGui",
		"ECS"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HD_PLATFORM_WINDOWS"
		}

		prebuildcommands
		{
			"{COPY} ../Dlls/ ../bin/" .. outputdir .. "/%{prj.name}"
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
