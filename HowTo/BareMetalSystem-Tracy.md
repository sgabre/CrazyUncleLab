# Tracy Software on a Bare Metal System

"Tracy" software in embedded systems typically refers to tracing tools or mechanisms that allow you to monitor and debug a system's execution in real-time or post-mortem. Implementing tracing in a bare-metal system involves capturing and possibly transmitting information about system behavior, such as function calls, execution times, interrupts, or state changes.

## Features

Be able to send some trace over the Debug Port or ITM.

When the Button B1 is press, then the message "Button B1 has been press".

**Extention** (Not Implemeted in this version, it will be see on the Error Managements )

When a error occurs a message is sent over the Debug Port/ITM, the message have the followig format

- "Error <#Error>, occurs on <fileName> @ line <#Lines>."

This message can be send over Debug Port/ITM or logs in a files.

This message can be store on the "Special recovery Memory"

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
