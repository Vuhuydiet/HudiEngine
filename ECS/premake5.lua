project "ECS"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ecspch.h"
	pchsource "src/ecspch.cpp"

	files
    {
        "src/**.h",
        "src/**.cpp"
    }

	includedirs
	{
		"src"
	}

	filter "system:windows"
	systemversion "latest"

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
