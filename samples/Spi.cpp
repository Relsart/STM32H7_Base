#include <stdint.h>
#include "Signal.h"
#include "driver/spi/Spi.h"
#include "driver/dma/Stream.h"
#include "Loger.h"
#include "driver/Dwt.h"

/**
 * @brief Sandbox
 */
void SPI_Sample()
{
    // ONLY for MASTER mode yet...

    // Prepare different datas:
    const uint32_t packSize = 0xFF;
    uint8_t txData1[packSize];
    uint16_t txData2[packSize];
    uint32_t txData4[packSize];

    uint8_t rxData1[packSize];
    uint16_t rxData2[packSize];
    uint32_t rxData4[packSize];

    for (int i = 0; i < packSize; i++)
    {
        txData1[i] = i;
        txData2[i] = 1000 + i;
        txData4[i] = 1000000 + i;
        rxData1[i] = 0xFF;
        rxData2[i] = 0xFFFF;
        rxData4[i] = 0xFFFFFFFF;
    }

    // Create and configurate SPI instance:
    driver::spi::Spi spi1(SPI1);
    driver::spi::Config spiconf = 
    {
        .mode = driver::spi::DeviceMode::Master,        // Master mode
        .firstBit = driver::spi::FirstBit::Msb,         // Order MSB first
        .dirMode = driver::spi::WorkMethod::TwoLines,   // Full duplex
        .speedDiv = driver::spi::FreqPrescaler::Div16,  // SCK frequency
        .nssMode = driver::spi::CsMode::Soft,           // NSS signal software control
        .sckPol = driver::spi::Level::Low,              // SCK low level in idle
        .sckPhase = driver::spi::Phase::First,          // Signal capture at first front of SCK
        .masterRxAutoSuspend = true,
        .crcEnable = false,                             // CRC disabled
        .fifoThreshold = 1
    };
    spi1.init(spiconf);
    Log(lmSystem, Info) << "SPI1 speed = " << spi1.getSckFreq() << " Hz";   // Can get the frequency of SCK signal

    spi1.setMethod(driver::spi::TransferMethod::General);   // Not necessary- it's General by default

    // Transmit various datas:
    spi1.transmit(txData1, packSize);   // Sending bytes
    spi1.transmit(txData2, packSize);   // Sending half-words
    spi1.transmit(txData4, packSize);   // Sending words

    // Receive various datas:
    spi1.receive(rxData1, packSize);    // Reading bytes
    spi1.receive(rxData2, packSize);    // Reading half-words
    spi1.receive(rxData4, packSize);    // Reading words

    // Transceive (receiving and transmitting at the same time)
    spi1.transceive(txData1, rxData1, packSize);    // Exchanging bytes
    spi1.transceive(txData2, rxData2, packSize);    // Exchanging half-words
    spi1.transceive(txData4, rxData4, packSize);    // Exchanging words

    driver::DwtTimer::getInstance().delayMs(200);

    // Change to DMA mode:
    driver::dma::Stream txStream, rxStream;
    spi1.onDmaTxStream(&txStream);
    spi1.onDmaRxStream(&rxStream);
    spi1.setMethod(driver::spi::TransferMethod::Dma);

    // Repeat previously exersisez in DMA mode:
    spi1.transmit(txData1, packSize);
    spi1.transmit(txData2, packSize);
    spi1.transmit(txData4, packSize);
    spi1.receive(rxData1, packSize);
    spi1.receive(rxData2, packSize);
    spi1.receive(rxData4, packSize);
    spi1.transceive(txData1, rxData1, packSize);
    spi1.transceive(txData2, rxData2, packSize);
    spi1.transceive(txData4, rxData4, packSize);
}