## CMakeLists @ Track Directory 
cmake_minimum_required(VERSION 3.23)
project(Track LANGUAGES C VERSION 5.4.3)

#Options
option(BUILD_COMPONENT_TEST "Build Component Testing" OFF)
option(BUILD_COMPONENT_CLI "Build Command-In-Line Interfaces for the Track Software Component" ON)



if(BUILD_COMPONENT_TEST STREQUAL "ON")
	set(BUILD_COMPONENT_CLI ON)
endif()

message("BUILD_COMPONENT_TEST: " ${BUILD_COMPONENT_TEST})
message(" BUILD_COMPONENT_CLI: " ${BUILD_COMPONENT_CLI})

# Executable
if(BUILD_COMPONENT_CLI STREQUAL "ON")
	message("Build Command-In-Line Interfaces...")
	add_executable(Track sources/Track_CLI.c sources/Track.c  sources/Track.h)
	target_include_directories(Track PUBLIC "${PROJECT_BINARY_DIR}" )
endif()

#Test
if(BUILD_COMPONENT_TEST STREQUAL "ON")
	message("Build Component Testing...")
	enable_testing()
	function(ComponentTest target arg result)
		add_test(NAME "ComponentTest ${arg}" COMMAND ${target} ${arg})
		set_tests_properties("ComponentTest ${arg}" PROPERTIES PASS_REGULAR_EXPRESSION ${result})
	endfunction()
	
	ComponentTest(Track --version 0 "Track - Version: 0.0.0 - Revision: 0123456789ABCDEF - Build Date & Time: 27/07/1979 @ 10:30:45")
	
endif()


#if (MSVC)
#	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
#endif()

#add_library(Track SHARED)
#target_include_directories(Track PRIVATE "${PROJECT_SOURCE_DIR}")
#add_subdirectory("configuration")
#add_subdirectory("interfaces")
#add_subdirectory("sources")

#install(TARGETS Track 
#	EXPORT TrackTargets
#	FILE_SET HEADERS
#	LIBRARY DESTINATION lib
#	ARCHIVE DESTINATION lib
#	RUNTIME DESTINATION bin
#	INCLUDES DESTINATION include)

##	FILE TrackTargets.cmake
#	DESTINATION lib/cmake/Track)

#include(CMakePackageConfigHelpers)
#write_basic_package_version_file(
#	"TrackVersion.cmake"
#	VERSION ${SOFTWARE_COMPONENT_VERSION}
#	COMPATIBILITY AnyNewerVersion)

#install(FILES "TrackConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/TrackVersion.cmake"
#	DESTINATION lib/cmake/Track)

#mkdir longtitleproject && cd $_
#cmake ..
#make
#cmake --install . --prefix ../../geo-install
#cmake --install . --config Debug --prefix "/Users/sgabre/git/install"