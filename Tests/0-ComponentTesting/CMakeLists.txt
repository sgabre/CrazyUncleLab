#/TestHarness/UnitTesting/CMakeLists.txt

# Get all subdirectories under UnitTesting
file(GLOB CHILD_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

# Loop through them and add those with CMakeLists.txt
foreach(dir ${CHILD_DIRS})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/CMakeLists.txt")
        add_subdirectory(${dir})
    endif()
endforeach()
