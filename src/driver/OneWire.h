#pragma once
#include <stdint.h>
#include <stdio.h>
#include "stm32h7xx.h"
#include "Dwt.h"

namespace driver {

/**
 * @brief ROM serial structure of OneWire device
 */
struct __attribute__((packed)) OneWireDevROM
{
    uint8_t familyCode;
    uint8_t serial[6];
    uint8_t crc;
};

/**
 * @brief OneWire interface class  
 */
class OneWire
{
public:
    /**
     * @brief OneWire general commands codes
     */
    enum class Comamnds : uint8_t
    {
        SearchRom   = 0xF0,  // Search algorithm, which find out the unique 64-bit addresses of all devices on the bus
        SkipRom     = 0xCC,  // Accessing all devices at once
        MatchRom    = 0x55,  // Accessing a specific device by its 64-bit address
        ReadRom     = 0x33,  // Read the address if there is only one device on the bus
        AlarmSearch = 0xEC   // Search for devices that have an alarm flag triggered
    };

    /**
     * @brief Constructor
     * @param [in] GPIOx Port (GPIOA, GPIOB..)
     * @param [in] pin Pin number
     */
    OneWire(GPIO_TypeDef* GPIOx, uint8_t pin);

    /**
     * @brief Initialization (for GPIO periphery)
     */
    void init();

    /**
     * @brief Bus reset
     * @return True: there is at least one device on the bus, False: no devices
     */
    bool resetBus();

    /**
     * @brief Bus scan for connected devices ROM IDs
     * @param [out] serial Buffer for received ROM IDs
     * @param [in] maxDevCount Number of connected devices 
     * @return Number of successfully found devices
     */
    uint8_t scanBus(OneWireDevROM* serial, int maxDevCount);

    /**
     * @brief Transmit byte to the bus
     * @param [in] dataByte Data
     */
    void writeByte(uint8_t dataByte);

    /**
     * @brief Receive byte from the bus
     * @return Data
     */
    uint8_t readByte();

private:
    GPIO_TypeDef* const m_GPIOx;    // GPIO port
    const uint8_t m_pin;            // GPIO pin number
    const uint32_t m_pinMask;       // GPIO pin mask
    DwtTimer& m_delayer;            // Link for precisious us intervals generator

    /**
     * @brief Saved state for devices searching process
     */
    struct SearchState
    {
        uint8_t lastDiscrepancy = 0;
        uint8_t lastFamilyDiscrepancy = 0;
        bool lastDeviceFlag = false;
        uint8_t romIdValue[8]{0};
    };
    SearchState m_searchState;

    /**
     * @brief One bit receiving
     * @return Bit value
     */
    bool readBit();

    /**
     * @brief One bit transmitting
     * @param [in] bit Bit value
     */
    void writeBit(bool bit);

    /**
     * @brief One device ROM Id searching cycle (64 bits)
     * @details Temporary results and states saved to the SearchState structure
     * @return True: one device ROM Id successfully read; False: error or no more devices left
     */
    bool searchCycle();

    /**
     * @brief Reset the temporary results in SearchState structure
     */
    void searchReset();
};

}   // namespace driver
