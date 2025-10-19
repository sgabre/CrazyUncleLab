add_executable(ComponentTest-Board-TestSuite-Exe
    ComponentTest-Board-TestSuite.c
)

target_link_libraries(ComponentTest-Board-TestSuite-Exe
    module_a
    Unity
)

add_test(ComponentTest-Board-TestSuite ComponentTest-Board-TestSuite-Exe)
