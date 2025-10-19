## Firmware System testing

### Scope

The purpose of Component testing is to TBD

### Testbench

#### Hardware

- Power Supply
- J-Link
- Virtual COM/Debug Port (UART/ITM)
- Product Board/Device Board
- Raspberry
- External Sensors & Actuators (if applicable)

#### Software

1. Test Case Development
- doxygen
- IDE (VS Code, STM32CubeIDE),
  
2. Test Execution
- Compilator, 
- CMake
- Shelly
- Python
 
### Test Type

✅ Functional Testing – Validate that the firmware meets functional requirements in real-world scenarios.
✅ Interface Testing – Verify interactions between firmware and hardware interfaces.
✅ System Integration Testing – Ensure the entire firmware system operates as expected across integrated modules.
✅ End-to-End Testing – Test the firmware with all components interacting, including hardware, software, and external systems.
✅ Communication Testing – Verify UART, SPI, I2C, CAN, Ethernet, Bluetooth, or other communication protocols.
✅ Error Handling & Recovery Testing – Simulate failures and verify system recovery.
✅ Performance Testing – Measure CPU load, memory usage, execution time, and overall system responsiveness.
✅ Consumption Testing – Evaluate system power and memory efficiency under different conditions.
✅ Regression Testing – Ensure that changes do not break existing functionality.

### Inputs

- Integration Strategies
- Firmware Component Code Sources
- Firmware Architecture Design [Firmware Architecture, Functional Decoposition]
- Firmware Detailled Design 

### Delivery

- Code Sources
- SystemFirmware.bin
- Firmware Test Report
- Quality Report 

### Criterial

### Dashboard*
 
