# Introduction

le but c'est de reflechier et de definir qu'elle sont les étapes pour un projet embarque pour faire de l'intégration et de déploiement continus.

# Term & Definition

## Devise Software 

## Target Software 

## Software System

A software system is a broader term that refers to a collection of software components working together to achieve a specific goal or function. A software system can consist of multiple layers, including application software, middleware, libraries, and utilities.

## Software Component

A software component in the context of embedded systems is a modular, self-contained unit of software that provides specific functionality within a larger system. It is typically designed to be reusable, maintainable, and replaceable without significantly impacting the overall system. Components often communicate with each other through well-defined interfaces.

## Software Module

A software module is a self-contained, logically organized collection of software units (functions or methods) that work together to provide a specific, cohesive functionality or service within a larger system. A module is typically responsible for handling a particular aspect of the system, such as communication, data processing, or hardware control. It groups related software units into a single unit that can be developed, tested, and maintained independently of other modules in the system.



## Software Unit 

A software unit refers to the smallest testable part of a software system. It is often the fundamental building block of a larger system or module, and it typically consists of a single function or method that performs a specific task. In the context of embedded systems, a software unit is generally designed to encapsulate a single responsibility, making it independently testable and maintainable.

## Delivery Package

A Delivery Package refers to a collection of software components, documents, tools, and other necessary items bundled together and provided as a complete unit to a customer, stakeholder, or as part of a project. The contents of a delivery package are designed to enable the recipient to install, configure, or deploy the software in a specific environment, ensuring that everything needed for successful deployment and operation is included.

## Software Package

A Software Package is a collection of software components bundled together for distribution, installation, and use. It typically includes executable files, configuration files, libraries, documentation, and other resources needed to run or use a particular software system or application. The purpose of a software package is to provide a ready-to-install, self-contained unit of software that can be deployed or used without requiring additional software components or manual setup.

## Update Package

An Update Package refers to a collection of files, resources, and instructions designed to update an existing software system to a newer version. It typically contains new features, bug fixes, security patches, and sometimes changes in configuration or system settings. The purpose of an update package is to improve or enhance the software without requiring a complete reinstallation.

##  Configuration Package

A Configuration Package refers to a set of files or data that define the configuration settings and parameters for a system or application. It is used to customize the behavior or setup of software or hardware components, ensuring they operate according to specific user requirements or environmental conditions. In embedded systems or software development, a configuration package typically contains the information needed to tailor the system's settings without modifying the core software.



# Roadmap

- Software Component 
   * Build/Compile the component (*.a, *.lib, *.so, *.dll, ...)
   * Code Size reporter
   * Static Code Analysis
   * Build Software Detailled Design (Doxygen)
   * Unit Testing (Unity, CMock, ...)
   * Component Testing (Unity, CMock, ...)
   * Code Coverage (branche coverage, statement coverage, condition converage)
   * Install/Publish Pack (Source code , Debug Library, Release Library, inclusion)
- Build/Compile System Software
- Integration testing ()
    * Device Programming (J-Tag Flash )
    * Component integration testing
    * System integration testing
- System testing
- Acceptance testing

# Note


## What is a embedded system ? 

An embedded system is a dedicated computing system integrated into a device for a specific real-time function. It combines **software**, **hardware**, and **mechanics**, with performance and reliability constraints.

So basicaly, an embedded system is a device or a product.

## What are the phases of an Embedded System Project ? 

[INSERT IMAGE]

- Requirement Analysis
- System Designs
- Developments
- Testing and Validation
- Industrialization and Production
- Deployment and Maintenance

For each domain (**software**, **hardware**, and **mechanics**) in a Embedded System Project typically has its own Development Life Cycle.


## For the Software, What is the Development Life Cycle ? 

- Software development planning
- Software requirements analysis
- Software Architectural design
- Software detailed design
- Software Unit implementation and verification
- Software integration and integration testing
- Software System testing
- Software release

### Software development planning

- Process to be used in the development
- Deliverables
- Traceabilit between System Requirement, Software Requirement, Software System Test
- Software configuration and change management

### Software requirements analysis

### Software Architectural design

### Software detailed design

### Software Unit implementation and verification

### Software integration and integration testing

### Software System testing

###  Software Release

(CI/CD) = Compilation, Static Code Analysis, Metrix,  Flashing, Unit Testing, Component Testing, ... Packaging

Metrix
- Cyclomatic complexity
- Ligne de Code Sources
- Binaries Size (ROM, RAM)
- Code Coverage
- Number Of Test Case (Total, Pass, Fail, Not Executed)

Cyclomatic complexity Criteria

1 - 10: Simple procedure, little risk
11 - 20: More complex, moderate risk
21 - 50: Complex, high risk
> 50: Untestable code, very high risk

Type

- Weekly build/release
- Sprint build/release
- Milestone Build/release


## Verification & Validation
The difference between verification and validation is fundamental in engineering, software development, and quality assurance:

## Verification – "Are we building the product right?"

Ensures that the product is being developed correctly according to specifications, design documents, and requirements.
Focuses on processes such as reviews, inspections, static analysis, and testing at the component level.
Typically involves checking compliance with standards.

## Validation – "Are we building the right product?"

Ensures that the final product meets the user's needs and intended use.
Involves system-level testing, integration tests, acceptance tests, and real-world trials.
Focuses on functionality, performance, and usability to ensure the system behaves as expected in its actual operating environment.

## Verification Process

|Technique               |  Purpose     | Tools Used| 
| -------------          | ------------- |------------- |
| Static Code Analysis   | Detect coding violations (MISRA, CERT C)           |	Cppcheck, Coverity, PC-lint      |
| Unit Testing           | Validate individual software unit           |Unity (C), Google Test (C++), Ceedling      |
| Component  Testing     | Validate individual software components           |Unity (C), Google Test (C++), Ceedling      |
| Integration Testing    | Check interaction between software modules  | Hardware-in-the-Loop (HIL), CANoe, Robot Framework  |
| Requirement Traceability |Ensure all requirements are implemented |	DOORS, Polarion |
| Code Review & Walkthroughs | Ensure design and implementation correctness | GitHub Code Review, Gerrit |
| Formal Verification| Mathematical proof of correctness for safety-critical software | SPIN, TLA+, Model Checking |
| Simulation & Emulation |Test software on simulated hardware before real deployment | QEMU, Renode, Proteus|	


## Memory Verification

|Issue	|Detected by   | 
| -------------          | ------------- |
| Buffer Overflow	| Static & Dynamic Analysis | 
| Stack Overflow	| Dynamic Analysis & Stack Monitoring | 
| Memory Leaks	| Dynamic Analysis & Heap Tracking | 
| Null Pointer Dereference	| Static Analysis | 
| Dangling Pointers	| Dynamic Execution Tracing | 
| Uninitialized Memory Access	| Static Analysis | 
| Heap Corruption	| Dynamic Memory Checks | 

- Buffer Overflow: Occurs when a program writes more data to a buffer than it can hold, leading to memory corruption or security vulnerabilities.	
- Stack Overflow: Happens when a program exceeds the allocated stack memory, often due to deep recursion or excessive local variable use, causing crashes.	
- Memory Leak:	Occurs when a program allocates memory but never releases it, leading to increased memory usage over time and potential system slowdowns or crashes.	
- Null Pointer Dereference: Happens when a program attempts to read or write memory using a pointer that has not been initialized (i.e., points to NULL), leading to undefined behavior or crashes.
- Dangling Pointer: Occurs when a pointer still references memory that has already been freed, leading to unpredictable behavior, crashes, or data corruption.
- Uninitialized Memory Access: Happens when a program reads a variable before it has been assigned a value, leading to unpredictable or incorrect behavior.
- Heap Corruption: Occurs when a program writes outside allocated heap memory, modifying adjacent memory blocks and causing instability.



## Validation Process

|Technique               	|  Purpose     | Tools Used| 
| ------------------------------| ----------------------------------------------------------------------------- |--------------------------------------------- |
| System Testing		| Validate complete firmware, hardware, and software integration		| Automated Test Benches, LabView| 
| Functional Testing		| Check if all features work as expected					| Test scripts in Python, Squish| 
| Performance Testing		| Test system response time, power consumption, efficiency			| NI TestStand, Power Profiler Kit| 
| Stress & Load Testing		| Check system behavior under high data rates, long operation			| 	JMeter, Locust| 
| Field Testing			| Deploy in real-world conditions to verify meter readings, RF signal quality	| LoRa Gateway Monitoring, Drive Tests| 
| Compliance Testing		| Ensure compliance with industrial standards (IEC, DLMS, MID)			| Rohde & Schwarz, Keysight Spectrum Analyzers| 
| EMC Testing			| Verify electromagnetic compatibility in industrial environments		| Anechoic Chambers, EMC Test Labs| 
		
# Software Test Life Cycle 

## Requirement Analysis

📥 Inputs:

- Software Requirement Specification (SRS)
- Hardware Requirement Specification (HRS)
- Functional Safety Requirements (if applicable)
- Communication Protocol Specifications (I2C, SPI, CAN, UART, etc.)
- Power Consumption and Performance Requirements
- Compliance Standards (e.g., IEC 62304 for medical devices, DO-178C for avionics)
  
👤 Roles Involved:

- System Architect (Defines system requirements)
- Embedded Software Engineer (Understands firmware constraints)
- Test Manager / QA Lead (Defines testing scope)
  
📤 Deliverables:

- Requirement Traceability Matrix (RTM)
- Testable Functional and Non-Functional Requirements Document
- Hardware-Software Interface Test Requirements

## Test Planning

📥 Inputs:

- Test Requirements
- Embedded System Architecture
- Hardware Availability & Constraints
- Safety and Regulatory Compliance Needs

👤 Roles Involved:

- Test Manager / QA Lead
- Embedded Test Engineer
- Hardware Engineer

📤 Deliverables:

- Embedded Test Strategy Document
- Test Plan Document
- Effort Estimation & Resource Planning
- Hardware Setup Plan (e.g., Oscilloscope, Logic Analyzer, Debugger, etc.)
- Automation Feasibility Report
  
## Test Case Development

📥 Inputs:

- Test Plan
- Firmware APIs & Communication Interfaces
- Hardware Design Documents (schematics, PCB layout)

👤 Roles Involved:

- Embedded Software Test Engineer
- Firmware Developer
- Hardware Engineer

📤 Deliverables:

- Manual & Automated Test Cases (covering real-time performance, interrupts, power consumption, etc.)
- Test Scripts for Automation (e.g., Python for serial interface, LabVIEW for hardware-in-loop testing)
- Boundary Value Analysis & Equivalence Class Test Cases
- Test Data Sets (for hardware sensors, ADC/DAC, etc.)

## Test Environment Setup

📥 Inputs:

- Target Hardware (Prototype, Evaluation Board, Custom PCB)
- Debugger (e.g., J-Link, ST-Link)
- Test Bench Setup (Power supply, Oscilloscope, Logic Analyzer, Signal Generator)
- Software Tools (e.g., STM32CubeIDE, Keil, IAR, Segger RTT)

👤 Roles Involved:

- Embedded Test Engineer
- Hardware Engineer
- DevOps Engineer

📤 Deliverables:

- Test Environment Readiness Report
- Flashed & Configured Hardware Test Setup
- Connectivity & Communication Verification (UART, SPI, CAN, etc.)

## Test Execution (Embedded Context)

📥 Inputs:

- Test Cases
- Configured Test Environment
- Firmware Image (Binary/Hex File)

👤 Roles Involved:

- Embedded Software Test Engineer
- Firmware Developer
- Hardware Engineer

📤 Deliverables:

- Pass/Fail Test Execution Report
- Defect Reports with Root Cause Analysis (RCA)
- Real-Time Behavior Logs (Jitter, Response Time, Memory Leaks, etc.)
- Regression Testing Results
- Power Consumption & Stress Test Reports

## Test Cycle Closure (Embedded Context)

📥 Inputs:

- Test Execution Results
- Defect Reports
- Test Coverage Metrics
  
👤 Roles Involved:

- Test Manager
- Embedded Test Engineer
- Project Manager

📤 Deliverables:

- Final Test Summary Report
- Defect Density & Test Coverage Metrics
- Lessons Learned Document (for future projects)
- Archival of Test Artifacts (Logs, Test Scripts, Setup Configuration)
- Key Embedded System Testing Considerations
- Hardware-Software Integration Testing: Ensures firmware works with real hardware.
- Real-Time Performance Testing: Measures execution timing constraints and jitter.
- Power Consumption Testing: Ensures low power consumption in embedded systems.
- Interrupt & Exception Handling Testing: Verifies ISR execution timing and error handling.
- Protocol Testing: Validates communication via SPI, UART, CAN, I2C, etc.
- Compliance Testing: Ensures adherence to regulatory standards like IEC 62304 (medical), DO-178C (avionics), or ISO 26262 (automotive).

		
	
