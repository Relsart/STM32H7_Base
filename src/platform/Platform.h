#pragma once

#include "driver/gpio/Gpio.h"
#include "driver/SysTimer.h"
#include "driver/Crc.h"
#include "driver/spi/Spi.h"
#include "driver/can/Can.h"
#include "driver/i2c/I2c.h"
#include "driver/Rcc.h"
#include "driver/uart/Uart.h"
#include "driver/dma/Stream.h"
#include "driver/nvic/Nvic.h"
#include "driver/usb/Usb.h"
#include "driver/Dwt.h"
#include "driver/sdmmc/Sdmmc.h"
#include "Blinker.h"
#include "driver/OneWire.h"

using namespace driver;

namespace platform {

/**
 * @brief   ACHTUNG: Sources of all objects, declared in this class, must be compiled at the moment of platform compilation!
 *          For this purpose set them in target_link_libraries in CMakeLists.txt file.
 */

class Mcu
{
public:
    // GPIO pins:
    gpio::Pin boardLed;
    gpio::Pin boardButton;
    usb::Device usbDev;

    LedBlinker ledBlinker;  // On-board led

    // DMA streams:
    dma::Stream logStream;  // Stream for console logs
    dma::Stream i2cStream;
    
    // Communication ports and interfaces:
    uart::Port console;
    spi::Spi spi1;
    I2C i2cPort;
    OneWire m_oneWire;
    // can::Bus can1;

    // External memory:
    SdCard sdCard;

    /**
     * @brief Constructor.
     */
    Mcu ();

    /**
     * @brief Periphery initialization
     */
    void init ();
};

/**
 * @brief External peripheral devices class
 * @details Sensors, displays etc...
 */
class Peripheral
{
public:
    /**
     * @brief Constructor.
     */
    Peripheral();

    /**
     * @brief Periphery initialization
     * @return True == Ok
     */
    bool init();

private:
    friend class Mcu;
    Mcu* const m_Mcu; 
};


extern Mcu mcu;
extern Peripheral peripheral;

}   // namespace platform
