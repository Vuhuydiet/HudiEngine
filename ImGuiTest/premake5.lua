
project "ImGuiTest"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Hudi/vendor/SDL2/include",
		"%{wks.location}/Hudi/vendor/SDL2_image/include",
		"%{wks.location}/Hudi/vendor/ImGui",

		"src"
	}

	libdirs
	{
		"%{wks.location}/Hudi/vendor/SDL2/lib/x64",
		"%{wks.location}/Hudi/vendor/SDL2_image/lib/x64"
	}

	links 
	{
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib",
		"opengl32.lib",

		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"
		
		prebuildcommands
		{
			"{COPY} ../Dlls/ ../bin/" .. outputdir .. "/%{prj.name}"
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