# /TestHarness/Firmware/Module/CMakeLists.txt

set(MODULE_NAME "Dummy")

# Set source files for this module
SET(SOURCES
    Dummy.c
)

# Option to choose between shared and static library
option(BUILD_SHARED "Build as a shared library" OFF)

# Determine library name based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LIB_NAME "d${MODULE_NAME}Lib")
else()
    set(LIB_NAME "${MODULE_NAME}Lib")
endif()

# Define the library type based on the option
if(BUILD_SHARED)
    add_library(${MODULE_NAME} SHARED ${SOURCES})
else()
    add_library(${MODULE_NAME} STATIC ${SOURCES})
endif()

# Specify include directories
TARGET_INCLUDE_DIRECTORIES(${MODULE_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Set the library output name (ensures proper naming without 'lib' prefix)
set_target_properties(${MODULE_NAME} PROPERTIES OUTPUT_NAME ${LIB_NAME})

# Set output directories
set_target_properties(${MODULE_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Libraries/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Libraries/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Libraries/bin
)

# Install rules
install(TARGETS ${MODULE_NAME} 
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

# Ensure headers are installed properly
install(FILES Module.h DESTINATION ${CMAKE_INSTALL_PREFIX}/inc)


# Ensure parent directories are created first
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/Documents)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/Documents/FirmwareLowLevelDesign)
set(DOXYGEN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Documents/FirmwareLowLevelDesign/Module")
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

