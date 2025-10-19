# Firmware Test Plan Generator Prompt

**Mode:** `agent`  
**Description:** Generate a comprehensive Firmware Test Plan for embedded software projects.

---

## Task

Analyze the provided firmware specifications, architecture, and requirements, and generate a structured, detailed Firmware Test Plan that covers all aspects of testing, including functional, integration, system, and regression testing.

---

## Firmware Test Plan Structure

### 1. Project Overview
- `Firmware Name` / Version  
- `Project Manager / Lead`  
- `Test Plan Author`  
- `Start Date` and `End Date`  
- `Objective / Goal` of the test plan  
- Optional `Notes` or constraints  

### 2. Firmware Overview
- Brief description of the firmware functionality  
- Target hardware platforms and peripherals  
- Supported configurations / modes  

### 3. Test Scope
- Modules, functions, and features to be tested  
- Features out of scope (if any)  
- Test coverage goals (functional, integration, system-level)  

### 4. Test Strategy
- Testing types (unit, integration, system, regression, performance, stress, fault injection, security)  
- Test methods (manual, automated, simulation, hardware-in-the-loop)  
- Entry and exit criteria for each test type  

### 5. Test Environment
- Hardware setup (boards, sensors, peripherals)  
- Software setup (toolchain, OS, simulators, test frameworks)  
- Network or communication configuration (if relevant)  

### 6. Test Cases
For each test case:  
- `Test Case ID`  
- `Module / Feature`  
- `Objective / Description`  
- `Preconditions`  
- `Test Steps`  
- `Expected Result`  
- `Pass/Fail Criteria`  
- `Priority` / `Severity`  
- Optional `Notes`  

### 7. Resources
- Test engineers and roles  
- Required tools and equipment  
- Test data requirements  

### 8. Schedule
- Test start and end dates  
- Milestones for each test phase  
- Dependencies between tests  

### 9. Reporting
- Test logs and results format  
- Defect reporting and tracking process  
- Metrics to measure test coverage and quality  

---

## Requirements

- Include realistic and detailed test cases; do not use placeholders  
- Align test plan with the firmware architecture and requirements  
- Highlight critical, high-risk areas and regression tests  
- Provide a ready-to-use plan suitable for team review and execution  
- Focus on: `${input:focus_firmware:Which firmware or modules should be covered?}`  

Generate a **production-ready Firmware Test Plan** that can be used directly by QA engineers and developers.
