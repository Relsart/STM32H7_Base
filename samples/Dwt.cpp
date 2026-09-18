
#include <stdint.h>
#include "driver/Dwt.h"

/**
 * @brief Samples of DWT timer using, timeouts and delays:
 */
void foo()
{
    __disable_irq ();   // It works without IRQ enabling!

    // Simple second delay:
    driver::DwtTimer::getInstance().delayMs(1000);

    // TIMEOUT REALISATION:
    // At first get the parameter structure with timeout set:
    driver::DwtTimer::TimeoutData param = driver::DwtTimer::getInstance().timeoutInit(1000);

    // Timeout control value (counter) creates locally and can't be rewritten by other process, interruptions, handlers etc..
    // Timer counter for compare (stores and increments in private DWT counter) is never overwritten by any process, so it will not be broken 
    while (true)
    {
        // In cycle update and check timestamp parameters for timeout:
        if (driver::DwtTimer::getInstance().checkTimeout(param))
        {
            Log(lmSystem, Error) << "Timeout!"; // Time elapsed!
            return;
        }
    }
}
