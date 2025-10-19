add_executable(IntegrationTest-Midleware-Module-TestSuite-Exe
    IntegrationTest-Midleware-Module-TestSuite.c
)

target_link_libraries(IntegrationTest-Midleware-Module-TestSuite-Exe
    module_a
    Unity
)

add_test(IntegrationTest-Midleware-Module-TestSuite IntegrationTest-Midleware-Module-TestSuite-Exe)
