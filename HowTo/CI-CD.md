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
| Unit Testing           | Validate individual software components           |Unity (C), Google Test (C++), Ceedling      |
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
		




		
		
	
