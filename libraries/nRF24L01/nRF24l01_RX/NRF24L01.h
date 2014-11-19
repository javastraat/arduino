#ifndef NRF24L01_h
#define NRF24L01_h

#include "API.h"

//*********************************************
#define SPI_PORT PORTB
#define SPI_DIR  DDRB
#define SPI_IN   PINB
//---------------------------------------------
#define TX_ADR_WIDTH    5   
// 5 unsigned chars TX(RX) address width
#define TX_PLOAD_WIDTH  1  
// 20 unsigned chars TX payload
//---------------------------------------------
#define CE       0x01
// CE_BIT:   Digital Input     Chip Enable Activates RX or TX mode
#define SCK      0x04
// SCK BIT:  Digital Input     SPI Clock

#define MISO     0x10
// MISO BIT: Digital Output    SPI Slave Data Output, with tri-state option
#define CSN      0x02
// CSN BIT:  Digital Input     SPI Chip Select
#define MOSI     0x08
// MOSI BIT: Digital Input     SPI Slave Data Input
#define IRQ      0x20
// IRQ BIT:  Digital Output    Maskable interrupt pin
//*********************************************
#endif
