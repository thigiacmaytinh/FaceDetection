-------------------------------------------------------------------------------
-- CELib
-------------------------------------------------------------------------------
local my_path = path.getabsolute(_SCRIPT)
local extern_path = my_path .. "/../../../"

depends_on = depends_on or {}
-------------------------------------------------------------------------------
-- add all checkout folders for lib dependencies
-------------------------------------------------------------------------------
local jsoncpp_checkout_folder = custom_checkout_folders and custom_checkout_folders['jsoncpp'] or 'jsoncpp'

local lua_config_file = extern_path .. "jsoncpp/jsoncpp.lua"
if io.open(lua_config_file, 'r') then
	dofile(lua_config_file)
end

include (extern_path .. "premake/common/")
-------------------------------------------------------------------------------
-- lib solution
-------------------------------------------------------------------------------
if pre_build then
	pre_build()
end

solution "jsoncpp"
	startproject ("Test")
	addCommonConfig()

	
-------------------------------------------------------------------------------
-- add project ref files for lib dependencies
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
-- lib project
-------------------------------------------------------------------------------
project "jsoncpp"
	uuid "42ABB4F7-AE8A-3393-B7A2-BE6923019AF4"
	kind "StaticLib"
	targetname( "jsoncpp" )
	targetdir ( extern_path .. jsoncpp_checkout_folder .. "/lib/" .. GetPathFromPlatform())
-------------------------------------------------------------------------------
-- project files
-------------------------------------------------------------------------------	
	files 
	{ 
		"../source/*.cpp", 
		"../source/*.inl", 
		"../source/*.h", 
		"../include/json/*.h"
	}
	
-------------------------------------------------------------------------------
-- include paths
-------------------------------------------------------------------------------		

	
	includedirs { "../include" }
			
-------------------------------------------------------------------------------
-- add external lib dependencies(adds include paths, defines and links)
-------------------------------------------------------------------------------	

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-- project props/xcconfig
-------------------------------------------------------------------------------		
	if IsVisualStudio() then
	
		pchheader "stdafx.h"
		pchsource "../source/stdafx.cpp"
		
		configurationfiles( { 
			extern_path .. "jsoncpp_config/" .. _OPTIONS["to"] .. "/common.props"
		} )
	end
	
	if IsXCode() then
		generatexcconfigs "YES"
		
		-- setting these 'user' .xcconfig files we overwrite the default ones
		configurationfiles
		{
			extern_path .. "jsoncpp_config/" .. _OPTIONS["to"] .. "/user-debug-project.xcconfig",
			extern_path .. "jsoncpp_config/" .. _OPTIONS["to"] .. "/user-release-project.xcconfig",
			extern_path .. "jsoncpp_config/" .. _OPTIONS["to"] .. "/user-debug-target.xcconfig",
			extern_path .. "jsoncpp_config/" .. _OPTIONS["to"] .. "/user-release-target.xcconfig"
		  }		
	end
-------------------------------------------------------------------------------
-- platform specific settings
-------------------------------------------------------------------------------		
	if IsXCode() then
	
		addCommonXcodeBuildSettings()
		
		if (IsIos()) then
           defines {"OS_IPHONE"}       
		elseif (IsTvos()) then
			defines {"OS_APPLETV"}        
		end
	
-------------------------------------------------------------------------------
-- files that need to be compiled as Objective-C++
-------------------------------------------------------------------------------	
	end	

	if IsWin32() then		
		defines {"WIN32 OS_WIN32"}
	end
	
if post_build then	
	post_build()
end
-------------------------------------------------------------------------------
-- lib Test project
-------------------------------------------------------------------------------
project "Test"
	targetdir ( extern_path .. jsoncpp_checkout_folder .. "/tests/" .. GetPathFromPlatform())
	targetname( "Test" )

-------------------------------------------------------------------------------
-- The type of the entry point
-------------------------------------------------------------------------------
	if IsXCode() then
		kind "WindowedApp"
	else
		kind "ConsoleApp"
	end
-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------