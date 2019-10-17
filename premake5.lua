workspace "HazelGameEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory) 
IncludeDir = {}
IncludeDir["GLFW"] = "HazelGameEngine/3rdParty/GLFW/include"
IncludeDir["Glad"] = "HazelGameEngine/3rdParty/Glad/include"
IncludeDir["ImGui"] = "HazelGameEngine/3rdParty/imgui"
IncludeDir["glm"] = "HazelGameEngine/3rdParty/glm"

-- Include to the premake5.lua file (c++ style)
include "HazelGameEngine/3rdParty/GLFW"
include "HazelGameEngine/3rdParty/Glad"
include "HazelGameEngine/3rdParty/imgui"

project "HazelGameEngine"
	location "HazelGameEngine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "HazelGameEngine/src/hzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/3rdParty/glm/glm/**.hpp",
		"%{prj.name}/3rdParty/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/3rdParty/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		--staticruntime "On"
		systemversion "latest"
	
		defines
		{
			"HZ_BUILD_DLL",
			"HZ_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}
	--using runtime, it wont break poit in Hazel
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		buildoptions "/MDd"
		--runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		--buildoptions "/MD"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		--buildoptions "/MD"
		runtime "Release"
		symbols "On"



project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"HazelGameEngine/3rdParty/spdlog/include",
		"HazelGameEngine/src",
		"%{IncludeDir.glm}"
	}

	links
	{
		"HazelGameEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		--staticruntime "On"
		systemversion "latest"
	
		defines
		{
			"HZ_PLATFORM_WINDOWS"
		}

		
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		buildoptions "/MDd"
		--runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		--buildoptions "/MD"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		--buildoptions "/MD"
		runtime "Release"
		symbols "On"

	filter { "system:windows", "configurations:Release"}
		buildoptions "/MT"