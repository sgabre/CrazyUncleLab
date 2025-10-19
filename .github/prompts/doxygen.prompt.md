---
mode: 'agent'
description: 'Generate Doxygen documentation for C source code and headers'
---

## Task

Analyze the provided C code and generate clear, structured Doxygen documentation for all functions, macros, structs, enums, and global variables.

## Doxygen Structure

Generate documentation including:

1. **File / Module Header**
   - `@file` with filename
   - `@brief` summary of module purpose
   - `@author`
   - `@date`
   - Optional `@note` or `@warning` sections for special considerations

2. **Function Documentation**
   - `@brief` short description of what the function does
   - `@param` for each parameter, with type and description
   - `@return` description of return value (if any)
   - `@note` for special behaviors (thread-safety, memory management)
   - `@example` if usage example is relevant

3. **Structs and Enums**
   - `@struct` / `@enum` name and brief description
   - Document each member with `@var` or description

4. **Macros and Constants**
   - `@def` or `@brief` description
   - Explain purpose and usage

5. **Global Variables**
   - `@var` description, type, and purpose
   - Notes on usage constraints

## Requirements

- Use proper Doxygen syntax (`/** ... */`, `@param`, `@return`, etc.)
- Maintain the original code structure; **do not modify code**, only add documentation
- Include realistic descriptions, not placeholders
- Generate complete documentation even for functions/structs without comments
- Provide ready-to-paste documented code

Focus on: ${input:focus_code:Which C file(s) or functions should be documented?}

Generate **production-ready Doxygen comments** that can be used to generate HTML, PDF, or Markdown documentation with Doxygen.
