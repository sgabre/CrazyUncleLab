# Tracy Software on a Bare Metal System

"Tracy" software in embedded systems typically refers to tracing tools or mechanisms that allow you to monitor and debug a system's execution in real-time or post-mortem. Implementing tracing in a bare-metal system involves capturing and possibly transmitting information about system behavior, such as function calls, execution times, interrupts, or state changes.

## Features

Be able to send some trace over the Debug Port or ITM.

When the Button B1 is press, then the message "Button B1 has been press".

**Extention** (Not Implemeted in this version, it will be see on the Error Managements )

When a error occurs a message is sent over the Debug Port/ITM, the message have the following format

- "Error <#Error>, occurs on <fileName> @ line <#Lines>."
- "On INT <InterruptionShortName>, Error <#Error>, occurs on <fileName> @ line <#Lines>."
- "On <TaskName>, Error <#Error>, occurs on <fileName> @ line <#Lines>."

This message can be send over Debug Port/ITM or logs in a files.

- Timestamps;Sources;#Error;File;Line

This message can be store on the "Special recovery Memory"

# How to use a Max OS X for connecting through the Virtual COM Port with the Nucleo F401RE Board?

You'll need to get a usb-serial adapter and then connect with a console cable. Adapters aren't very expensive.

Connecting to the Console Port with Mac OS X
To connect a Mac OS X system USB port to the console using the built-in OS X Terminal utility, follow these steps:

- Step 1 Use the Finder to go to Applications > Utilities > Terminal.
- Step 2 Connect the OS X USB port to the router.
- Step 3 Enter the following commands to find the OS X USB port number:

```shell
  $> cd /dev
  $> ls -ltr /dev/*usb*
```
 

- Step 4 Connect to the USB port with the following command followed by the router USB port speed:

```shell
  $> screen /dev/tty.usbmodem1a21 9600
```
**Note:**
To Disconnect the OS X USB Console from the Terminal Window
Enter Ctrl+A followed by Ctrl+\

```shell
  $> cd /dev
  $> ls -ltr /dev/*usb*
```

# Command-In-Line

```shell
$> screen /dev/tty.usbserial-AQ02K49L 115200
$> screen /dev/cu.usbserial-AQ02K49L 115200
$> screen/dev/tty.usbmodem11103 115200 (ST-LINK Virtual COM Port)
$> screen /dev/cu.usbmodem11103 115200
```
## Files

The default structure when you are using STM32CubeMX is as follows: 

```
<Project Name>
\- STM32F401RE.ioc
\- STM32F401RETX_FLASH.ld
\- STM32F401RETX_RAM.ld
|- Core
|  \- Inc
|  \- Src
|  \- Startup 
|- Drivers
|  |- CMSIS
|  |  \- Device
|  |  \- Include
|  \- STM32F4xx_HAL_Driver
|  |  \- Inc
|  |  \- Src
```

## Hardware Mapping

### RCC

|Pin Number     |  Pin Name        | Signal On Pin   | Software Name  |
| ------------- | ------------- |------------------- |-------------   |
|3              |PC14-OSC32_IN  |RCC_OSC32_IN        | N/A            |
|4              |PC15-OSC32_OUT |RCC_OSC32_OUT       | N/A            |
|5              |PH0 - OSC_IN   |RCC_OSC_IN          | N/A            |
|6              |PH1 - OSC_OUT  |RCC_OSC_OUT         | N/A            |

### JTAG Port

|Pin Number  |  Pin Name     | Signal On Pin | Software Name  |
| ------------- | ------------- |------------- |-------------   |
|46           | PA13          |SYS_JTMS-SWDIO |N/A            |
|49           | PA14          |SYS_JTCK-SWCLK |N/A            |
|55           | PB3           |SYS_JTDO-SWO   |N/A            |

### Debug Port

|Pin Number  |  Pin Name     | Signal On Pin | Software Name  |
| ------------- | ------------- |------------- |-------------   |
|16           | PA2           |USART2_TX      |
|17           | PA3           |USART2_RX      |

### LED

|Pin Number  |  Pin Name     | Signal On Pin |  Software Name  |
| ------------- | ------------- |------------- |-------------   |
|21           | PA5           | LED2          | LED2

### Buttons

|Pin Number  |  Pin Name     | Signal On Pin |  Software Name  |
| ------------- | ------------- |------------- | -------------   |
|2            | PC13          | PC13-ANTI_TAMP| B1 |
