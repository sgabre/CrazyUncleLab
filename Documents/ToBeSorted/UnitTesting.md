# What is a Unit Test in embedded Software ?



# Why is the goal of Unit Test in embedded Software ?

The goal of the  Unit Tests is to Help Detect 

- Logic errors (incorrect algorithm implementation)
- Data handling errors (type conversion issues, incorrect data processing)
- Exception handling issues (poor error management)
- Unhandled edge cases (extreme values or special conditions)
- State inconsistencies (incorrect modifications to an object's state)
- Performance issues (if measured)
- API design flaws (incorrect input or output types)
- Regressions (previously fixed errors reappearing)
- Interaction issues with other components (e.g., database access problems)
- Missing or invalid values (invalid input or incorrect behavior with null values)

# Which technic can i use to write a Unit Test  ?

## Basic Level

- Equivalence partitioning testing
- Boundary testing
- State transition testing
- Use case testing
- Error testing

## Advance Level
- Exploratory Testing (Advance)
  * Architectural diagrams and use cases
  * Past defects
  * Error handling
  * Guesses
  * Discussions
- Error guessing (Advance)

# What are the Best Practices for Unit Testing ?

- Cyclomatic complexity
- Apply the 5W1H Approach (What, When, Where, Who, Why, How)
- Follow the FIRST Principles (Fast, Isolated, Repeatable, Self-validating, Timely)
- One Test = One Case (Keep Tests Simple and Focused)
- Follow the AAA Pattern (Arrange - Act - Assert)
- Use Mocks and Stubs to Isolate Components

# Unit Testing Criteria ?

A Unit Testing Criteria is used to determine if the individual units of a software system function correctly in isolation. 
It ensures that each function and by extention module behaves as expected under different conditions.

| Type	| Embedded Systems	|
|------|------------------|
| Function coverage: how many of the functions defined have been called. | |
| Statement coverage: how many of the statements in the program have been executed.	| 85–100%	|
| Branches coverage: how many of the branches of the control structures (if statements for instance) have been executed.	| 75–100%	|
| Condition coverage: how many of the boolean sub-expressions have been tested for a true and a false value.	| 75–100%	|
| Line coverage: how many of lines of source code have been tested.|90-100% | 

# How to make a Unit Testing ?





