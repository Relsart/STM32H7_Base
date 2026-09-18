# STM32H743 Base project

## Description

Base project for developing on STM32H743 chips. Includes periphery drivers (uart, spi, can, etc.., will be updated), external modules (slot-signal data exchange template, loger, ring buffers etc..),
some external libraries (etl).

## Installation

Compiling and linking with Cmake and GCC arm-none-eabi toolchain (tested well at 10.3.1 version).

## Periphery drivers

In current version drivers for the periphery devices have supported:
- [ ] GPIO with EXTI interruptions handling and debounce filtering
- [ ] UART/USART (general and DMA modes)
- [ ] SPI for Master mode yet (general and DMA modes)
- [ ] I2C for Master mode yet (beta version, in process...)
- [ ] Can bus (only for extended ID yet)
- [ ] System periphery (System and DWT timers, CRC module, etc..)

## Advanced features

- [ ] Slot-signals template libraries, Loger and others in Modules library (external submodule)
- [ ] Terminal commands processing mechanism (under development yet)

## Using external libraries

- [ ] Etl library (https://github.com/ETLCPP/etl.git)
- [ ] FatFS file system library (https://elm-chan.org/fsw/ff/)
- [ ] FreeRTOS (https://github.com/FreeRTOS)