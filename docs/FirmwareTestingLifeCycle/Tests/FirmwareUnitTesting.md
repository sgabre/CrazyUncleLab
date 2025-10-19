## Firmware Unit Testing

### Scope

The purpose of unit testing is to verify individual units (including both public and private functions) to ensure they work as expected in isolation. It helps detect and fix defects early in the development cycle, thereby improving code quality, reliability, and maintainability.

The purpose of unit testing is to verify individual functions (public and private) to ensure they work as expected in isolation. It helps detect and fix defects early in the development cycle, thereby improving code quality, reliability, and maintainability.

### Testbench

#### Hardware

- Power Supply: Provides the necessary power to the microcontroller board during testing.
- J-Link: A debug probe for communication with the microcontroller, enabling debugging and flashing.
- Debug Port (UART/ITM): Communication interface used for receiving test results from the device, enabling monitoring and debugging. 
- Board(s): The hardware platform running the firmware under test, which could be a specific development or production board used for testing.

#### Software

1. Test Case Development

- Doxygen: Documentation generation tool to create detailed descriptions of the codebase and individual functions.
- IDE (e.g., VS Code): Integrated development environment for writing, editing, and managing test cases.
  
2. Test Execution

- Compiler: Converts the source code and test cases into executable files.
- CMake: Build system to manage and automate the compilation and testing process.
- Unity: Unit testing framework designed for C/C++ that simplifies the creation and execution of test cases.
- CMock: Mocking library used in conjunction with Unity to create mocks for functions, allowing isolation of the unit under test.
- Ceedling: A testing framework that integrates Unity and CMock, providing an environment for testing and organizing test cases.
- Terminal: Command-line interface to execute tests and view results.
- Code Coverage (gcov): A tool that tracks which parts of the code are executed during testing. 

### Test Type

* Boundary Tests - Test function behavior at limit values.
* Structural Tests – Ensure code coverage requirements (statement/branch coverage).
* Functional Tests - Verify correct output based on expected behavior.
* Error Handling Tests – Validate function responses to invalid inputs.
* Performance Tests – Measure execution time, memory usage, and CPU cycles.
* Regression Tests – Ensure previous functionality remains intact (Testbench + CI)

### Inputs

- "Firmware Module" Code Sources
- Firmware Low Level Design/Firmware Detailled Design (doxygen for each function(s))

### Delivery

- Unit Test Sources: Test cases for each function.
- FunctionUnitTestRunner.bin: Compiled test runner executable.
- Unit Test Report: Pass/fail status for each test case.
- Quality Report: Overall test coverage, cyclomatic complexity, and code quality metrics.

### Criterial

-  Number of Total Unit Test is greater or equal to Cyclomatic complexity
-  For each Function, the Statement Coverage greater than 70%
-  For each Function, the Branch Coverage greater than 70%

### Dashboard*

| Function                | Cyclomatic complexity | # Total | # Pass | # Fail | # Not Executed |
|-------------------------|-----------------------|---------|--------|--------|----------------|
| Module_PublicFunction   |   12                  |     14  |   12   |  1     |   1            |
| Module_PrivateFunction  |   10                  |     5   |   3     |  1    |   3            | <= Issue

| Function                | Statement Coverage    | Branch Coverage |
|-------------------------|-----------------------|-----------------|
| Module_PublicFunction   |   100 %               |         100 %   |
| Module_PrivateFunction  |   100 %               |         100 %   |

| Function                              | Code Size (Byte)      | RAM (Byte)            |
|---------------------------------------|-----------------------|-----------------------|
| Module_PublicFunctionUnitTestRunner   |   1024                |   35                  |
| Module_PrivateFunctionUnitTestRunner  |   54                  |   64                  |


| Function	                            | Execution Time (ms)	 | Memory Usage (Bytes)	 | Code Size (Bytes) | CPU Cycles    |
|---------------------------------------|-----------------------|-----------------------|-------------------|---------------|
| Module_PublicFunctionUnitTestRunner   |	1.2 ms	             | 512 B	                | 1024 B	         | 24000         |
| Module_PrivateFunctionUnitTestRunner  | 0.8 ms	             | 256 B	                | 768 B             | 18000         |


Note:

- Measuring stack usage with -fstack-usage
- Runtime analysis with stack painting
- Runtime analysis with GDB single-stepping
- Tracking Firmware Code Size
- puncover
- Bounds Checking on Buffers
- Memory Leak Detection in an RTOS Environment
- Static Memory Analysis
- Custom memory management
- Stack Overflow Checking
 
