#/TestHarness/UnitTesting/ModuleSuite/CMakeLists.txt

# Find all unit test source files ending with _Suite.c
file(GLOB SUITE_SOURCES *_Suite.c)

# Loop over each test source and create a runner
foreach(SUITE_FILE ${SUITE_SOURCES})
    # Get the file name without extension and path
    get_filename_component(SUITE_NAME ${SUITE_FILE} NAME_WE)

    # Create executable
    add_executable(${SUITE_NAME} ${SUITE_FILE})

    # Link with your test dependencies
    target_link_libraries(${SUITE_NAME} PRIVATE Module Unity)

    # Register test with CTest
    add_test(NAME ${SUITE_NAME} COMMAND ${SUITE_NAME})

    # Optional: install it under bin/tests
    install(TARGETS ${SUITE_NAME} RUNTIME DESTINATION bin/tests)
endforeach()


