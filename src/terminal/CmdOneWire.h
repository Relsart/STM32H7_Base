#pragma once
#include "TermCmdInterface.h"
#include "Loger.h"

namespace console {

/**
 * @brief Command for OneWire bus scan and read devices ROM serial codes.
 *        Command name: onewire
 *        Arguments: one (optional) is max connected devices number. Default is 1.
 */
class CmdOneWireScan : public CmdInterface
{
private:
    const uint8_t m_MaxDevices = 100;

public:
    /**
     * @brief Console command handler
     */
    void exec(uint32_t argc, char** arg) override;

    /**
     * @brief Constructor 
     */
    CmdOneWireScan() : CmdInterface("onewire", "Scan OneWire bus for devices ROM serials")
    {}
};

extern CmdOneWireScan cmdOneWireScan;

}   // namespace console
