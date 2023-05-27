project "HudiAPI"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "src/pch.cpp"

	defines
	{
		"HD_API_EXPORTS",
		"_CRT_SECURE_NO_WARNINGS"
	}

	files
    {
        "src/**.h",
        "src/**.cpp",

		"vendor/glm/**.hpp",
		"vendor/glm/**.inl"
    }

	includedirs
	{
		"%{wks.location}/Hudi/vendor/spdlog/include",
		"%{wks.location}/Hudi/vendor/glm",

		"src",
		"%{wks.location}/ECS/src"
	}

	links
	{
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
			"{COPY} %{wks.location}/Dlls/ %{wks.location}/bin/" .. outputdir .. "/%{prj.name}"
		}

		postbuildcommands
		{
			"{COPY} %{wks.location}/bin/" .. outputdir .. "/HudiAPI/HudiAPI.dll %{wks.location}/bin/" .. outputdir .. "/Hudi",
			"{COPY} %{wks.location}/bin/" .. outputdir .. "/HudiAPI/HudiAPI.dll %{wks.location}/bin/" .. outputdir .. "/HudiGen"
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
