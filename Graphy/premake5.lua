project "Graphy"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
		"%{wks.location}/Engine/Engine",
	}

	links 
	{
		"Engine"
	}

	files
	{
		"**.cpp",
		"**.h",
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		defines "DEBUG=1"
		kind "ConsoleApp"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		defines "RELEASE=1"
		kind "WindowedApp"
		optimize "on"