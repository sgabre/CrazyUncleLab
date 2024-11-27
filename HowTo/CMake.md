# Features (Draft)

- Create a library from Software Component
- Create a binary from a set of Software Component
- Create a firmware image from a set of binary
- Create a binary from a set of Software Component
- be able to do that on different target(Windows, Linux, Mac OS X, RTOS, BMS)
- Have a Command-In-Line to test it
- have a build scripts to compile, generate packages, test, configuration, ...
- have a script to import some software component

# Data/file structures

```
. SoftwareComponent
|- configuration
|  \- (entry point of the main application)
|- interfaces
|  \- (Software Component sources)
|- sources
|  \- (Software Component sources)
```
#### **Step 1: Compile the `<SoftwareComponent>`**

1. **Set Up the Environment**:
   - Ensure that the appropriate development tools and compilers are installed for the target platform (e.g., Visual Studio for Windows).
   - Set up environment variables to point to the correct toolchain for the build (e.g., for cross-compiling to embedded systems or RTOS).

2. **Configuration File**:
   - The configuration directory contains the `<SoftwareComponent>`_config.h file. This file uses configuration wizard annotations, which are special markers used to easily enable or disable features and set values in the configuration file.
     
   - Edit the `configuration/<SoftwareComponent>_config.h` to enable or disable features, as needed. For example:
     ```c
     //*** <<< Use Configuration Wizard in Context Menu >>> ***
     // <h> Feature Configuration
     //   <e1.1> Enable Feature X
     //     <o1.0..1> FEATURE_X_ENABLED: Enable/Disable Feature X <0=> Disabled <1=> Enabled
     //                 <i> Select whether to enable Feature X in the build
     #define FEATURE_X_ENABLED 1 // Enable Feature X
     // </h>
     //*** <<< end of configuration section >>> ***
     ```

3. **Run the Build Script**:
   - The script will compile the source files from the `sources` directory. The entry points for the main application are located here (e.g., `main.cpp`, `AppMain.c`, etc.).
   - The `interfaces` directory contains all the necessary header files that are needed by external software to use the `<SoftwareComponent>` library.

4. **Build Deliverables**

4.1. **Windows Build Deliverables**:
   - After compilation, the script generates the following output files:
     - **`<SoftwareComponent>_CLI.exe`**: A command-line executable for Windows, created from the sources in the `sources` directory.
     - **`<SoftwareComponent>.dll`**: A dynamic link library (DLL) containing the shared code, ready to be used by other applications on Windows.
     - **`<SoftwareComponent>.lib`**: A static library containing compiled object code, ready to be linked with other applications.
     - **Header files from the `interfaces` directory**: The header files located in the `interfaces` directory are copied to the output folder. These files are essential for any external software that needs to interact with or use the `<SoftwareComponent>` library.

4.2. **Mac OS X Build Deliverables**:

 - After compilation, the script generates the following output files:
     - **`<SoftwareComponent>_CLI`**: A command-line executable for Mac OS X, created from the sources in the `sources` directory.
     - **`<SoftwareComponent>.dylib`**: A dynamic shared library (DYLIB) created for Mac OS X containing the shared code, ready to be used by other applications.
     - **`<SoftwareComponent>.a`**: A static library containing compiled object code, ready to be linked with other applications.
     - **Header files from the `interfaces` directory**: The header files located in the `interfaces` directory are copied to the output folder. These files are essential for any external software that needs to interact with or use the `<SoftwareComponent>` library.

4.3. **Linux Build Deliverables**:

**TBD**

4.4. **RTOS Build Deliverables**:

**TBD**

6. **Output Locations**:
   - The compiled binaries (executables, DLLs, and static libraries) are placed in appropriate directories such as `build/` or `output/`.
   - Additionally, the header files from the `interfaces` directory are copied to the output folder (e.g., `output/include/`) so that external software can include and use them.
   - You can customize the output directory paths in the script for better organization.
  
#  References

- CMake Official Documentation
- CMake Tutorials

# Glossary

CMakeLists.txt: A file used by CMake to define the project configuration, source files, and build settings.
