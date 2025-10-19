## CMakeLists @ LSTime Directory 
cmake_minimum_required(VERSION 3.23)
project(LSTime LANGUAGES C VERSION 5.4.3)

#Options
option(BUILD_COMPONENT_TEST "Build Component Testing" OFF)
option(BUILD_COMPONENT_CLI "Build Command-In-Line Interfaces for the LSTime Software Component" ON)



if(BUILD_COMPONENT_TEST STREQUAL "ON")
	set(BUILD_COMPONENT_CLI ON)
endif()

message("BUILD_COMPONENT_TEST: " ${BUILD_COMPONENT_TEST})
message(" BUILD_COMPONENT_CLI: " ${BUILD_COMPONENT_CLI})

# Executable
if(BUILD_COMPONENT_CLI STREQUAL "ON")
	message("Build Command-In-Line Interfaces...")
	add_executable(LSTime sources/LSTime_CLI.c sources/LSTime.c  sources/LSTime.h)
	target_include_directories(LSTime PUBLIC "${PROJECT_BINARY_DIR}" )
endif()

#Test
if(BUILD_COMPONENT_TEST STREQUAL "ON")
	message("Build Component Testing...")
	enable_testing()
	function(ComponentTest target arg result)
		add_test(NAME "ComponentTest ${arg}" COMMAND ${target} ${arg})
		set_tests_properties("ComponentTest ${arg}" PROPERTIES PASS_REGULAR_EXPRESSION ${result})
	endfunction()
	
	
	#CLI ./LSTime 					Expected: Local Sidereal Time: 1.74046 @ LNG: 0 @ 01/01/1968 00:00:00
	#add_test(NAME "Component Testing #1" COMMAND LSTime)
	
	#CLI ./LSTime --lng 1  			Expected: Local Sidereal Time: 2.74046 @ LNG: 1 @ 01/01/1968 00:00:00
	ComponentTest(LSTime --lng 0 "Local Sidereal Time: 2.74046 @ LNG: 0 @ 01/01/1968 00:00:00")
	
	#CLI ./LSTime --date 1/01/1979  Expected: Local Sidereal Time: 1.74046342 @ LNG: 0 @ 01/01/1968 00:00:00
	ComponentTest(LSTime "--date 1/01/1979" "Local Sidereal Time: 1.74046342  @ LNG: 1 @ 01/01/1968 00:00:00")
	
	#CLI ./LSTime --date 1/01/1969  Expected: Local Sidereal Time: 1.75349981 @ LNG: 0 @ 01/01/1969 00:00:00
	ComponentTest(LSTime "--date 1/01/1969" "Local Sidereal Time: 1.75349981 @ LNG: 0 @ 01/01/1969 00:00:00")
	
	#CLI ./LSTime --date 1/01/1970  Expected: Local Sidereal Time: 1.74933340 @ LNG: 0 @ 01/01/1970 00:00:00
	ComponentTest(LSTime "--date 1/01/1970" "Local Sidereal Time: 1.74933340 @ LNG: 0 @ 01/01/1970 00:00:00")
	
	#CLI ./LSTime --date 1/01/1971  Expected: Local Sidereal Time: 1.74516701 @ LNG: 0 @ 01/01/1971 00:00:00
	ComponentTest(LSTime "--date 1/01/1971" "Local Sidereal Time: 1.74516701 @ LNG: 0 @ 01/01/1971 00:00:00")
endif()


if (MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

#add_library(LSTime SHARED)
#target_include_directories(LSTime PRIVATE "${PROJECT_SOURCE_DIR}")
#add_subdirectory("configuration")
#add_subdirectory("interfaces")
#add_subdirectory("sources")

#install(TARGETS LSTime 
#	EXPORT LSTimeTargets
#	FILE_SET HEADERS
#	LIBRARY DESTINATION lib
#	ARCHIVE DESTINATION lib
#	RUNTIME DESTINATION bin
#	INCLUDES DESTINATION include)

#install(EXPORT LSTimeTargets
#	FILE LSTimeTargets.cmake
#	DESTINATION lib/cmake/LSTime)

#include(CMakePackageConfigHelpers)
#write_basic_package_version_file(
#	"LSTimeVersion.cmake"
#	VERSION ${SOFTWARE_COMPONENT_VERSION}
#	COMPATIBILITY AnyNewerVersion)

#install(FILES "LSTimeConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/LSTimeVersion.cmake"
#	DESTINATION lib/cmake/LSTime)

#mkdir longtitleproject && cd $_
#cmake ..
#make
#cmake --install . --prefix ../../geo-install
#cmake --install . --config Debug --prefix "/Users/sgabre/git/install/Sample/SoftwareComponent"