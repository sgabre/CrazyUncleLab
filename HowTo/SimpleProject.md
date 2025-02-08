# Create Simple Project for the Nucleo-FR401RE Board

## Goal

It is a first project using the default setting of the STM32CubeMX It just to check if the tools chaine is working properly

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

## Mapping

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
|2            | PC13          | PC13-ANTI_TAMP| B1



|Platform        |  Architecture  | Toolchain      |
|----------------|----------------|----------------|
|Pi 1, Pi Zero, Pi Zero W	| ARMv6	| gcc-arm-linux-gnueabihf |
|Pi 2, Pi 3 (32-bit)	|ARMv7	| gcc-arm-linux-gnueabihf |
|Pi 3 (64-bit), Pi 4, Pi 5	| ARMv8 (AArch64)	| gcc-aarch64-linux-gnu |
|STM32F401RE | ARM | gcc-arm-none-eabi.cmake |
