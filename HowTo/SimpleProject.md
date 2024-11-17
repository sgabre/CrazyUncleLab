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

|Pin Number  |  Pin Name     | Signal On Pin | 
| ------------- | ------------- |------------- |
|3           |PC14-OSC32_IN  |RCC_OSC32_IN   |
|4           |PC15-OSC32_OUT |RCC_OSC32_OUT  |
|5           |PH0 - OSC_IN   |RCC_OSC_IN     |
|6           |PH1 - OSC_OUT  |RCC_OSC_OUT    |

### JTAG Port

|Pin Number  |  Pin Name     | Signal On Pin | 
| ------------- | ------------- |------------- |
|46           | PA13          |SYS_JTMS-SWDIO |
|49           | PA14          |SYS_JTCK-SWCLK |
|55           | PB3           |SYS_JTDO-SWO   |

### Debug Port

|Pin Number  |  Pin Name     | Signal On Pin | 
| ------------- | ------------- |------------- |
|16           | PA2           |USART2_TX      |
|17           | PA3           |USART2_RX      |

### LED

|Pin Number  |  Pin Name     | Signal On Pin | 
| ------------- | ------------- |------------- |
|21           | PA5           | LED2          |

### Buttons

|Pin Number  |  Pin Name     | Signal On Pin | 
| ------------- | ------------- |------------- | 
|2            | PC13          | PC13-ANTI_TAMP|
