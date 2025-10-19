## Firmware Component Testing

### Scope

The purpose of Component Testing is to verify the correctness, reliability, and performance of individual firmware components in isolation before integration into the complete system. This ensures that each component meets functional, structural, and quality requirements.

### Testbench

#### Hardware

- Power Supply
- J-Link
- Virtual COM/Debug Port (UART/ITM)
- Microcontroller Board/Dev Board/Product Board/Device Board

#### Software

1. Test Case Development
- doxygen – Documentation of the component and module design
- IDE (VS Code, STM32CubeIDE),
  
2. Test Execution
- Compiler – (GCC, IAR, Keil, etc.)
- CMake – Build System
- Unit Testing Frameworks – Unity, CMock, Ceedling
- Terminal – For logging and debugging

### Test Type

- Boundary Tests – Validate component behavior at min/max input values
- Functional Tests – Ensure correct output based on expected behavior
- Structural Tests – Verify Statement Coverage and Branch Coverage
- Error Handling Tests* – Test handling of invalid inputs and edge cases
- Performance Tests* - Measure execution time, memory usage, and CPU load
- Regression Tests – Ensure previous functionality remains intact (Testbench + CI)

### Inputs

- Firmware Component Code Sources
- Firmware Low Level Design/Firmware Detailed Design (doxygen for the Component & Module(s) )

### Delivery

* Component Test Code Sources – Component test implementations
* Module.lib – Library file for the tested module
* ModuleTestRunner.bin – Executable test runner for module testing
* ComponentTestRunner.bin – Executable test runner for component testing
* Unit Test Report – Detailed pass/fail results
* Quality Report – Coverage metrics, performance, and complexity analysis

### Criterial



### Dashboard*
 
