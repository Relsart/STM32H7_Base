#include "CmdOneWire.h"
#include "driver/OneWire.h"
#include "Functions.h"
#include "platform/Platform.h"

namespace console {

CmdOneWireScan cmdOneWireScan;

void CmdOneWireScan::exec (uint32_t argc, char** arg)
{
    uint8_t devCount = 1;
    if (arg && argc > 0)
    {
        int32_t temp = 0;
        if (stringToInt(arg[0], temp))
        {
            if (temp > 0 && temp <= m_MaxDevices)
                devCount = temp;
        }
    }

    Log() << Log::endl << Log::Base::Dec << "Search OneWire bus for " << devCount << " device(s), ";
    i_face::I_OneWire::DeviceRomId rom[devCount]{0};
    uint8_t found = platform::mcu.m_oneWire.scanBus(rom, devCount);
    Log() << Log::Base::Dec << "found: " << found << Log::endl;

    for (uint8_t i = 0; i < found; i++)
    {
        Log() << Log::Base::Dec << "Device " << (uint16_t)i << ": " << Log::Base::Hex << (uint16_t)rom[i].romStruct.familyCode << \
              " | " << (uint16_t)rom[i].romStruct.serial[0] << " " << (uint16_t)rom[i].romStruct.serial[1] << " " << (uint16_t)rom[i].romStruct.serial[2] << \
              " " << (uint16_t)rom[i].romStruct.serial[3] << " " << (uint16_t)rom[i].romStruct.serial[4] << " " << (uint16_t)rom[i].romStruct.serial[5] << \
              " | " << (uint16_t)rom[i].romStruct.crc << Log::endl;
    }
}

}   // namespace console
