## Firmware System Integration Testing (Draft)
### Scope

The purpose of Firmware Integration Testing is to verify that individual firmware components/modules interact correctly when integrated. It ensures proper data flow, communication, and functionality across modules while detecting issues like interface mismatches, incorrect dependencies, and integration errors.

### Testbench

#### Hardware

- Power Supply
- Power Measurement Equipment (e.g., Power Profiler Kit, Oscilloscope, Multimeter)
- J-Link
- Virtual COM/Debug Port (UART/ITM)
- Product Board/Device Board
- Raspberry

#### Software

1. Test Analysis & Planning
* Define test objectives, strategies, and test coverage requirements
3. Test Case Development
  * doxygen - Documentation for firmware architecture and module design
  * IDE (VS Code, STM32CubeIDE),
  
3. Test Execution
* Compiler – (GCC, IAR, Keil, etc.)
* CMake – Build System
* Shell Scripting (Shelly) – Automate test execution
* Python – Custom test scripts and automation frameworks
 
### Test Type

* Interface Testing – Validate interactions between firmware modules
* Data Flow Testing – Verify data consistency and correctness across interfaces
* Communication Testing – Test UART, SPI, I2C, CAN, or other communication protocols
* Error Handling Tests – Ensure proper error detection and recovery at integration level
* Performance Tests – Measure execution time, memory usage, and CPU load in integrated execution
* Consumption Testing – Evaluate the power and memory consumption of the firmware in different conditions:
1. Idle Power Consumption – Measure power draw in low-power states
2. Active Power Consumption – Measure power draw under various workloads
3. Peak Current Analysis – Identify peak current draw and power spikes
4. Memory Usage Analysis – Measure RAM and Flash usage
* Regression Tests – Ensure new changes do not break existing functionality (Testbench + CI)

### Inputs

* Firmware Integration Strategies – Define how components will be integrated and tested
* Firmware Component Code Sources – Individual Component, modules and drivers
* Firmware Architecture Design – Software architecture and functional decomposition
* Firmware Detailed Design – Interface definitions, state machines, and functional descriptions

### Delivery

* Component Test Code Sources – Test implementations for each component
* Module.lib – Compiled library for tested modules
* ModuleTestRunner.bin – Executable test runner for module testing
* ComponentTestRunner.bin – Executable test runner for component testing
* Integration Test Report – Summary of pass/fail results for each integration test case
* Quality Report – Detailed analysis of test coverage, performance, and integration effectiveness

### Criterial

### Dashboard*


Module-Level Integration Summary

| Integrated Modules        	| Test Cases	| Pass	| Fail| 	Not Executed |
|----------------------------|------------|------|-----|---------------|
| Communication (UART, I2C)	 |12	         | 10	  | 2   |	0             |
| Sensor Data Processing	    |8	          |7	    |1	   |0              |
| Memory Management	         |6	          |6	    |0	   |0              |

Consumption Testing Results

| Condition          |	Power Consumption (mW)	| Current Draw (mA)	 | RAM Usage (Bytes)	| Flash Usage (Bytes) |
|----------------------------|------------|------|-----|---------------|
| Idle Mode	         | 10	                    | 2	                 |64                 |	4096                |
| Active Mode	       | 150	                   | 30	                | 128	              | 8192                | 
| Peak Current Draw	 | 300	                   | 60	                | 256	              | 12288               | 

 
