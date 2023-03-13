project "CodeGenerator"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/")
    objdir ("bin-int/")

	pchheader "cgpch.h"
	pchsource "src/cgpch.cpp"

	files
    {
        "src/**.h",
        "src/**.cpp"
    }

	includedirs
	{
		"src"
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
