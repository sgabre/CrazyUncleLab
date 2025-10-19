# Production Requirements

This section defines the requirements necessary for the firmware to support production, calibration, and final testing activities.

> - The firmware shall support a **production test mode** that enables manufacturing personnel to execute diagnostics without accessing clinical functions.
> - All **calibration routines** shall be executable via a dedicated interface and provide pass/fail results automatically.
> - The system shall support **serial number programming** and configuration of production-specific parameters (e.g., hardware variant flags).
> - Production tools must be able to communicate with the firmware through a **dedicated protocol interface** (e.g., UART, USB, or CAN).
> - The firmware shall include **protection mechanisms** to prevent unauthorized re-entry into production mode after final device sealing or labeling.

---
