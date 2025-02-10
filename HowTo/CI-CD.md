# Introduction

le but c'est de reflechier et de definir qu'elle sont les étapes pour un projet embarque pour faire de l'intégration et de déploiement continus.

# Therm & definition

System Software: (Bootloader, Application)
Software Component: (Board Component)
Delivery Package
Software Package
Update Package
Configuration Package


# Roadmap

- Software Component 
   * Compilation
   * Installation
   * Unit Testing (Unity, CMock, ...)
   * Component Testing (Unity, CMock, ...)
- Compiler Bootloader
- Compiler Application
- Integration testing ()
    * Component integration testing
    * System integration testing
- System testing
- Acceptance testing

# Note

The difference between verification and validation is fundamental in engineering, software development, and quality assurance:

## Verification – "Are we building the product right?"

Ensures that the product is being developed correctly according to specifications, design documents, and requirements.
Focuses on processes such as reviews, inspections, static analysis, and testing at the component level.
Typically involves checking compliance with standards like IEC 62304 (medical software), DO-178C (avionics), or ISO 26262 (automotive).

## Validation – "Are we building the right product?"

Ensures that the final product meets the user's needs and intended use.
Involves system-level testing, integration tests, acceptance tests, and real-world trials.
Focuses on functionality, performance, and usability to ensure the system behaves as expected in its actual operating environment.


## Verification Process

Technique	Purpose	Tools Used
Static Code Analysis	Detect coding violations (MISRA, CERT C)	Cppcheck, Coverity, PC-lint
Unit Testing	Validate individual software components	Unity (C), Google Test (C++), Ceedling
Integration Testing	Check interaction between software modules	Hardware-in-the-Loop (HIL), CANoe, Robot Framework
Requirement Traceability	Ensure all requirements are implemented	DOORS, Polarion
Code Review & Walkthroughs	Ensure design and implementation correctness	GitHub Code Review, Gerrit
Formal Verification	Mathematical proof of correctness for safety-critical software	SPIN, TLA+, Model Checking
Simulation & Emulation	Test software on simulated hardware before real deployment
