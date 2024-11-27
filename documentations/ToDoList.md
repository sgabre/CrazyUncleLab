# To Do List

**Note**: Add the github reference when the item is done 

- [X] Create Simple Project for the Nucleo-FR401RE Board [V0.0.0](https://github.com/sgabre/CrazyUncleLab/releases/tag/V0.0.0)
- [ ] Create Blinky Software for the Nucleo-FR401RE Board with a Bare Metal System
- [ ] Create Tracy Software for the Nucleo-FR401RE Board with a Bare Metal System
- [ ] Add FreeRTOS
- [ ] Create Blinky Software for the Nucleo-FR401RE Board with RTOS (FreeRTOS)
- [ ] Create Tracy Software for the Nucleo-FR401RE Board with RTOS (FreeRTOS)
- [ ] Create Shelly Software for the Nucleo-FR401RE Board with RTOS (FreeRTOS)
- [ ] Compile with CMake for the  Nucleo-FR401RE Board
- [ ] Document the source code with docoxygen (the architecture shall be documented use module ...) 
- [ ] Create a shell script to Create a Keil Package (Sotware Component Pack and Board Support Pack )
- [ ] Create a shell script to Import a Keil Package (Sotware Component Pack and Board Support Pack )
- [ ] Define The project structure
- [ ] Add Unity Framework
- [ ] Add CLib
- [ ] Add Unit & Component Test for CLib
- [ ] Add FATFS
- [ ] Add MBEDTLS
- [ ] Add IoT
- [ ] Add bluetooth

# Milestone

1. Blinky (Faire clignote un led)
2. Instrumentation Trace Macrocell Trace 
3. Tracy (Envoie un text a traver le debug port et avec un printf)
4. Error (gestion des errors, assert, hard fault, …)
5. Shelly
6. CMake
7. Bord support package (Nucleo-F401RE)
8. Bootloader
9. Upgrader
10. Production
11. Application
12. Memory Management (Local/External RAM/Flash)
13. File System
14. Shell File Transfer
15. Bluetooth
16. Over-The-Air (OTA)
17. OTA Shell
18. Ethernet Driver
19. Internet Of Think (IOT) 
20. Power-On Self-Test (MCU,…)
21. Servo-Motor Driver
22. Elevation Driver
23. Azimuth Driver
24. Focus Driver
25. Power Manager
26. Final State Machine / Hirachic State Machine
27. 
# What i Want to do...

**Note**

I take what i can found on the shelf

## Simple Project for the Nucleo-FR401RE Board [V0.0.0](https://github.com/sgabre/CrazyUncleLab/releases/tag/V0.0.0)

It is a first project using the default setting of the STM32CubeMX
It just to check if the tools chaine is working properly

## Blinky Software for the Nucleo-FR401RE Board with a Bare Metal System

Using the Simple Project for the Nucleo-FR401RE Board, i want to blink led
- inside the main loop
- with a interuption (e.g: every 500 ms)

## Tracy Software for the Nucleo-FR401RE Board with a Bare Metal System

Using the Blinky Project for the Nucleo-FR401RE Board, i want to send data on a Computer Terminal (e.g: Hello Word)

## Blinky Software for the Nucleo-FR401RE Board with a RTOS

Using the Simple Project for the Nucleo-FR401RE Board, i want to blink led
- inside the main loop
- with a interuption (e.g: every 500 ms)

## Tracy Software for the Nucleo-FR401RE Board with a RTOS

Using the Blinky Project for the Nucleo-FR401RE Board, i want to send data on a Computer Terminal (e.g: Hello Word)

## Shelly Software for the Nucleo-FR401RE Board with a RTOS

Using the Blinky/Tracy Project for the Nucleo-FR401RE Board, i want to have a simple shell with the following command-in-line at minima
- Help: to get the command list available
- info/version/ver: to get the board information and the software version
- reboot: to reboot the software

