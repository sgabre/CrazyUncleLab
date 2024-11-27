# Features

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
