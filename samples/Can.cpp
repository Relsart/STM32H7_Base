#include <stdint.h>
#include "driver/can/Can.h"
#include "Loger.h"

/**
 * @brief Class-consumer CAN messages
 */
class TestCanReceiver : public SlotInterface <driver::can::Message*>
{
public:
    void run (driver::can::Message* msg, uint32_t) override
    {
        Log(lmSystem, Info) << "Can message received: ID " << Log::Base::Hex << msg->id << " size " << Log::Base::Dec << (uint16_t)msg->length << ", Data " <<
        msg->data[0] << " " << msg->data[1] << " " << msg->data[2] << " " << msg->data[3] << " " << msg->data[4] << " " << msg->data[5] << " " << msg->data[6] << " " << msg->data[7];    
    }
};

/**
 * @brief Sandbox
 */
void CAN_Sample()
{
    /* ----------------------- Init the CAN device instance ----------------------- */

    driver::can::Bus can1(FDCAN1);  // Create the CAN bus instance
    can1.init(1000);                // Init it with bitrate (in kbit/s), or use the overloaded init method (for extended params)

    /* -------- Init any Consumer instance and Can Filter (Data receiving) -------- */

    TestCanReceiver testCanReceiver;            // Some slot-consumer
    driver::can::Filter testFilter(0x03, 0xFF); // Can filter instance (id value + mask)
    testFilter.connect(&testCanReceiver);       // Connect filter (its signal) to target slot
    can1.addFilter(testFilter);                 // And ONLY AFTER THAT connect filter to Can bus! 
    // Now packets with ID 0x******03 will be receiving at testCanReceiver.run()

    /* ----------------------------- Data transmitting ----------------------------- */

    driver::can::Message canMsg
    {
        .id = 0xABCD,
        .length = 8
    };  // Prepare the Header
    canMsg.data[0] = 123;   // Put some data...
    can1.send(canMsg);      // And send to the bus    
}
