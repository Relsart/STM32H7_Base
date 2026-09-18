#include <stdint.h>
#include "driver/uart/Uart.h"
#include "driver/dma/Stream.h"

#include "Loger.h"
#include "Signal.h"
#include "driver/Dwt.h"

/**
 * @brief Receiver slot
 */
class UartConsumer : public SlotInterface <RingBuffer*>
{
public:
    uint8_t data[100]{0};   // Rx data buffer
    uint32_t getBytes = 0;  // Rx data counter
    bool getAll = false;    // Get all data flag

    void run (RingBuffer* buffer, uint32_t count) override
    {
        Log(lmSystem, Info) << "Size of incoming data: " << buffer->unreadedSize();
        // Read received data to own buffer, use it, etc...
        uint32_t read = buffer->read(&data[getBytes], sizeof(data));
        getBytes += read;
        if (getBytes >= 10)
            getAll = true;  // pack received!
    }

    void reset()
    {
        getAll = false;
        getBytes = 0;
    }
};

/**
 * @brief Sandbox
 */
void UART_Sample()
{
    uint8_t data[]{1, 2, 3, 4, 5, 6, 7, 8, 9};  // Data sample
    UartConsumer consumer;

    driver::uart::Port uartPort(USART2, 128);   // Create an object: set UART and size of Rx ring buffer
    uartPort.init(921600);                      // Set baudrate
    uartPort.setSlotInstantRx(&consumer);       // Connect to received data handler class
    uartPort.setTxMode(driver::uart::Mode::General);          // Set general mode
    uartPort.setRxMode(driver::uart::Mode::General);

    // transmit 2 datablocks:
    uartPort.transmit(data, sizeof(data));
    driver::DwtTimer::getInstance().delayMs(1000);
    uartPort.transmit(data, sizeof(data));
    driver::DwtTimer::getInstance().delayMs(1000);

    // Receive 10 bytes via general receiver:
    while (!consumer.getAll)
    {}

    // Switch receiver to DMA mode
    driver::dma::Stream rxStream;
    uartPort.onRxStream(&rxStream);
    uartPort.setRxMode(driver::uart::Mode::Dma);
    uartPort.setDmaRxBufferSize(100, false);    // allocate 100 bytes to DMA receiving buffer (single-buffered mode)
    uartPort.receiveViaDma(10, false);          // receive 10 bytes as single (non-circular) transaction

    // Receive 10 bytes via DMA receiver (single-buffered mode):
    consumer.reset();
    while (!consumer.getAll)
    {}

    // Receive 10 bytes via DMA receiver (double-buffered mode):
    uartPort.setDmaRxBufferSize(100, true);     // re-allocate DMA receiving buffer (double-buffered mode)
    uartPort.receiveViaDma(2, true);            // receive 10 bytes (5 * 2-bytes circular transactions)

    consumer.reset();
    while (!consumer.getAll)
    {}

    // Switch back to general RX mode:
    uartPort.setRxMode(driver::uart::Mode::General);
    consumer.reset();
    while (!consumer.getAll)
    {}

    // Transmit data via DMA stream:
    driver::dma::Stream txStream;
    uartPort.onTxStream(&txStream);     // Connect stream
    uartPort.setTxMode(driver::uart::Mode::Dma);      // Set DMA tx mode

    // Here can be tested multipackets transactions. DMA stream support up to 65535 data units (here- bytes).
    // UART DMA works around these limitation- it sends data in several packs.
    // To check this functionality change (for now!) const maxStreamTransactionSize (in dma/Stream.h) from 0xFFFF to (for example) 10
    // Data must be sended correct in multipacket mode:
    uint8_t bigData[256];
    for (int i = 0; i < sizeof (bigData); i++)
        bigData[i] = i;

    uartPort.transmit(bigData, sizeof(bigData));
    while (uartPort.dmaTxIsBuisy())
    {}
    uartPort.transmit(bigData, sizeof(bigData));
    while (uartPort.dmaTxIsBuisy())
    {}
}