# Blinky Software on a Bare Metal System

"blinky" software, which makes an LED blink on a RTOS, is a classic first step in embedded programming. 
This simple task demonstrates control over GPIO (General-Purpose Input/Output) pins and the ability to create time delays. 

## Features

Every 500 ms, the LED2 Change his state (On to Off or Off to On)

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
