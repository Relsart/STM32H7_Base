#pragma once
#include <stdint.h>
#ifdef WITH_ETL
#include "etl/delegate.h"
#else
#include <functional>
#endif

namespace i_face
{

/**
 * @brief One Wire bus interface
 */
class I_OneWire
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
     * @brief Unique device ROM id (8 bytes) with Family Code and CRC
     */
    union DeviceRomId
    {
        struct __attribute__((packed)) RomStruct
        {
            uint8_t familyCode;
            uint8_t serial[6];
            uint8_t crc;
        } romStruct;
        uint8_t romArray[8];
        uint64_t romPack;
    };

    /**
     * @brief Transmit byte to the bus
     */
    virtual void writeByte(uint8_t dataByte) = 0;

    /**
     * @brief Receive byte from the bus
     */
    virtual uint8_t readByte() = 0;

    /**
     * @brief Bus reset
     * @return True: there is at least one device on the bus, False: no devices
     */
    virtual bool resetBus() = 0;

    /**
     * @brief Read one time slot (1 bit)
     */
    virtual bool readBit() = 0;

    /**
     * @brief One bit transmitting
     */
    virtual void writeBit(bool bit) = 0;

    /**
     * @brief Checksum calculation
     */
    virtual uint8_t calcCrc(uint8_t* data, uint32_t size) = 0;
};

/**
 * @brief Timer interface
 */
class I_Timer
{
public:
    #ifdef WITH_ETL
    using Callback = etl::delegate<void()>;
    #else
    using Callback = std::function<void()>;
    #endif

    /**
     * @brief Timer working modes: one-shot or persistent
     */
    enum class WorkingMode { Once, Periodic };

    /**
     * @brief Subscribe for timers callback
     */
    void setCallback(Callback h) { m_callback = h; } 

    /**
     * @brief Start the timer
     * @param [in] mode Once or Periodic
     * @param [in] time Time value
     */
    virtual void start(uint32_t time, WorkingMode mode = WorkingMode::Once) = 0;

    /**
     * @brief Stop the timer
     */
    virtual void stop() = 0;
    
protected:
    Callback m_callback;
    WorkingMode m_mode;
};

}   // namespace i_face
