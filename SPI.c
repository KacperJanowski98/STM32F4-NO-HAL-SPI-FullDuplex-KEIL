#include "SPI.h"
#include "RccConfig.h"

void SPIConfig (void) {
	/*********** STEPS TO FOLLOW ************
	1. Enable SPI clock
	2. Configure the Control Register 1
	3. Configure the Control Register 2
	*****************************************/
	
	// 1. (Documentation 187s.)
	RCC->APB2ENR |= (1<<12);	// Enable SPI1 Clock
	// 2. (Documentation 917s.)
	SPI1->CR1 |= (1<<0) | (1<<1);	// CPOL=1, CPHA=1
	
	SPI1->CR1 |= (1<<2);	// Master Mode
	
	SPI1->CR1 |= (3<<3);	// BR[2:0] = 011: fPCLK/16 -> PCLK2 = 80 MHz, SPI clk = 5MHz
	
	SPI1->CR1 &= ~(1<<7);	// LSBFIRST = 0, MSB first
	
	SPI1->CR1 |= (1<<8) | (1<<9);	// SSM=1, SSi=1 -> Software Slave Management
	
	SPI1->CR1 &= ~(1<<10);	// RXONLY = 0, full-duplex
	
	SPI1->CR1 &= ~(1<<11);	// DFF = 0, 8 bit data
	// 2. (Documentation 918s.)
	SPI1->CR2 = 0;
}