#pragma once
#include "Signal.h"
#include "driver/SysTimer.h"
#include "Loger.h"

namespace platform {

/**
 * @brief Led-blinker control class
 * @details Indicator of MCU correct working
 */
class LedBlinker
{
private:
    #ifndef WITH_RTOS
    SoftTimer m_timer;  // Simple main-loop timer
    #else
    // TODO: RTOS timer here
    #endif

    driver::gpio::Pin& m_pin;   // GPIO pin link

    /**
     * @brief Blink signal handler
     */
    void blink()
    {
        m_pin.invert();
    }

public:
    /**
     * @brief Constructor
     */
    LedBlinker(driver::gpio::Pin& pin) : m_pin(pin)
    {
        m_timer.setCallback(SoftTimer::Callback::create<LedBlinker, &LedBlinker::blink>(*this));
        m_timer.start(500, i_face::I_Timer::WorkingMode::Periodic);
    }
};

}   // namespace platform
