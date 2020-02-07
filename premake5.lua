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
IncludeDir["stb_image"] = "HazelGameEngine/3rdParty/stb_image"

-- Include to the premake5.lua file (c++ style)
include "HazelGameEngine/3rdParty/GLFW"
include "HazelGameEngine/3rdParty/Glad"
include "HazelGameEngine/3rdParty/imgui"

project "HazelGameEngine"
	location "HazelGameEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "HazelGameEngine/src/hzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/3rdParty/stb_image/**.h",
		"%{prj.name}/3rdParty/stb_image/**.cpp",
		"%{prj.name}/3rdParty/glm/glm/**.hpp",
		"%{prj.name}/3rdParty/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/3rdParty/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"
	
		defines
		{
			"HZ_BUILD_DLL",
			"HZ_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
			"IMGUI_API=__declspec(dllexport)"
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
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		--buildoptions "/MD"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		--buildoptions "/MD"
		runtime "Release"
		symbols "on"



project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"HazelGameEngine/3rdParty",
		"%{IncludeDir.glm}"
	}

	links
	{
		"HazelGameEngine"
	}

	filter "system:windows"
		systemversion "latest"
	
		defines
		{
			"HZ_PLATFORM_WINDOWS"
		}

		
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		buildoptions "/MDd"
		--runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		--buildoptions "/MD"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		--buildoptions "/MD"
		runtime "Release"
		symbols "on"

	filter { "system:windows", "configurations:Release"}
		buildoptions "/MT"