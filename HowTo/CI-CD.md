# Introduction

le but c'est de reflechier et de definir qu'elle sont les étapes pour un projet embarque pour faire de l'intégration et de déploiement continus.

# Therm & definition

- Software System: A software system is a broader term that refers to a collection of software components working together to achieve a specific goal or function. A software system can consist of multiple layers, including system software (like bootloaders and RTOS), application software, middleware, libraries, and utilities.
- Software Component: (Board Component)
- Delivery Package
- Software Package
- Update Package
- Configuration Package
- Buffer Overflow: Occurs when a program writes more data to a buffer than it can hold, leading to memory corruption or security vulnerabilities.	
- Stack Overflow: Happens when a program exceeds the allocated stack memory, often due to deep recursion or excessive local variable use, causing crashes.	
- Memory Leak:	Occurs when a program allocates memory but never releases it, leading to increased memory usage over time and potential system slowdowns or crashes.	
- Null Pointer Dereference: Happens when a program attempts to read or write memory using a pointer that has not been initialized (i.e., points to NULL), leading to undefined behavior or crashes.
- Dangling Pointer: Occurs when a pointer still references memory that has already been freed, leading to unpredictable behavior, crashes, or data corruption.
- Uninitialized Memory Access: Happens when a program reads a variable before it has been assigned a value, leading to unpredictable or incorrect behavior.
- Heap Corruption: Occurs when a program writes outside allocated heap memory, modifying adjacent memory blocks and causing instability.


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


# Memory Verification

|Issue	|Detected by   | 
| -------------          | ------------- |
| Buffer Overflow	| Static & Dynamic Analysis | 
| Stack Overflow	| Dynamic Analysis & Stack Monitoring | 
| Memory Leaks	| Dynamic Analysis & Heap Tracking | 
| Null Pointer Dereference	| Static Analysis | 
| Dangling Pointers	| Dynamic Execution Tracing | 
| Uninitialized Memory Access	| Static Analysis | 
| Heap Corruption	| Dynamic Memory Checks | 



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
		




		
		
	
