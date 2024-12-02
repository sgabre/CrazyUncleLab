# Bluetooth Software 

TBD

## Features

TBD

## Hardware Mapping
Microcontroller: STM32F401RE
### RCC

|Pin Number     |  Pin Name        | Signal On Pin   | Software Name  |
| ------------- | ------------- |------------------- |-------------   |
|3              |PC14-OSC32_IN  |RCC_OSC32_IN        | N/A            |
|4              |PC15-OSC32_OUT |RCC_OSC32_OUT       | N/A            |
|5              |PH0 - OSC_IN   |RCC_OSC_IN          | N/A            |
|6              |PH1 - OSC_OUT  |RCC_OSC_OUT         | N/A            |

### JTAG Port

|Pin Number     |  Pin Name     | Signal On Pin | Software Name  |
| ------------- | ------------- |------------- |-------------   |
|46             | PA13          |SYS_JTMS-SWDIO |N/A            |
|49             | PA14          |SYS_JTCK-SWCLK |N/A            |
|55             | PB3           |SYS_JTDO-SWO   |N/A            |



### Debug Port

|Pin Number     |  Pin Name     | Signal On Pin | Software Name  |
| ------------- | ------------- |------------- |-------------   |
|16             | PA2           |USART2_TX      |
|17             | PA3           |USART2_RX      |

### LED

|Pin Number      |  Pin Name     | Signal On Pin |  Software Name  |
| -------------  | ------------- |------------- |-------------   |
|21              | PA5           | LED2          | LED2

### Buttons

|Pin Number      |  Pin Name     | Signal On Pin |  Software Name  |
| -------------  | ------------- |-------------  | -------------   |
|2               | PC13          | PC13-ANTI_TAMP| B1 |



### Buetooth

|Pin Number      |  Pin Name     | Signal On Pin |  Software Name  | 
| -------------  | ------------- |-------------  | -------------   | 
|?               | **PA0** or PC7| SPI_IRQ       | BLE_SPI_IRQ     | 
|?               | **PA1** or PB6| SPI_CSN       | BLE_SPI_CSN     | 
|?               | PA6           | SPI_MISO      | BLE_SPI_MISO    | 
|?               | PA7           | SPI_MOSI      | BLE_SPI_MOSI    | 
|?               | PA5           | SPI_CLK       | BLE_SPI_CLK     |
|?               | PA8           | BlueNRG_RST   | BLE_RST         | 
|?               | PA9           | SPI_EEPROM_nS | TBD             | 


## I2C

I2C1_SCL PB8 D15 CN5.10
I2C1_SDA PB9 D14 CN5.9


# SPI

SPI1_SCK  PA5 D13 CN5.6
SPI1_MISO PA6 D12 CN5.5
SPI1_MOSI PA7 D11 CN5.4
SPI1_CS   PB6 D10 CN5.3

# UART

USART2_TX PA2 D1 CN9.2
USART2_RX PA3 D0 CN9.1




