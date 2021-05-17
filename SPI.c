#include "SPI.h"

void SPIConfig (void) {
	/*********** STEPS TO FOLLOW ************
	1. Enable SPI clock
	2. Configure the Control Register 1
	3. Configure the Control Register 2
	*****************************************/
	
	// 1. (Documentation 187s.)
	RCC->APB2ENR |= (1<<12);	// Enable SPI1 Clock
	// 2. (Documentation 917s.)
	SPI1->CR1 |= (1<<0); 
	SPI1->CR1 |= (1<<1);	// CPOL=1, CPHA=1
	
	SPI1->CR1 |= (1<<2);	// Master Mode
	
	SPI1->CR1 &= ~(3<<3);	// BR[2:0] = 000: fPCLK/2 -> PCLK2 = 80 MHz, SPI clk = 5MHz
	
	SPI1->CR1 &= ~(1<<7);	// LSBFIRST = 0, MSB first
	
	SPI1->CR1 |= (1<<8) | (1<<9);	// SSM=1, SSi=1 -> Software Slave Management
	
	SPI1->CR1 &= ~(1<<10);	// RXONLY = 0, full-duplex
	
	SPI1->CR1 &= ~(1<<11);	// DFF = 0, 8 bit data
	// 3. (Documentation 918s.)
	SPI1->CR2 = 0;
}

void GPIOConfig (void) 
{
	RCC->AHB1ENR |= (1<<0);		// Enable GPIO A clock
	
	GPIOA->MODER |= (2<<10) | (2<<12) | (2<<14) | (1<<18);		// Alternate functions for PA5, PA6, PA7 and output for PA9
	
	GPIOA->OSPEEDR |= (3<<10) | (3<<12) | (3<<14) | (3<<18);		// High speed for PA5, PA6, PA7 and output for PA9
	
	GPIOA->AFR[0] |= (5<<20) | (5<<24) | (5<<28);		// AF5(SPI1) for PA5, PA6, PA7 
}

void SPI_Enable (void)
{
	SPI1->CR1 |= (1<<6);		// SPE=1, Peripheral enabled
}

void SPI_Disable (void)
{
	SPI1->CR1 &= ~(1<<6);		// SPE=0, Peripheral Disabled
}

void CS_Enable (void)
{
	GPIOA->BSRR |= (1<<9)<<16;
}

void CS_Disable (void)
{
	GPIOA->BSRR |= (1<<9);
}

void SPI_Transmit (uint8_t *data, int size)
{
	/*********** STEPS TO FOLLOW ************
	1. Wait for the TXE bit to set in the Status Register
	2. Write the data to the Data Register
	3. After the data has been transmitted, wait for the BSY bit to reset in Status Register
	4. Clear the Overrun flag by reading DR and SR
	*****************************************/
	
	int i=0;
	while (i<size)
	{
		while (!((SPI1->SR)&(1<<1))) {;}		// wait for TXE bit to set -> This will indicate that the buffer is empty
		SPI1->DR = data[i];		// Load the data into the Data Register	
			i++;
	}
	
	/*During discontinuous communications, there is a 2 APB clock period delay between the    (895s.)
	write operation to SPI_DR and the BSY bit setting. As a consequence, it is mandatory to 
	wait first until TXE=1 and then until BSY=0 after writing the last data.*/
	
	while (!((SPI1->SR)&(1<<1))) {;}		// wait for TXE bit to set -> This will indicate that the buffer is empty
	while (((SPI1->SR)&(1<<7))) {;}			// wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
	
	// Clear the Overrun flag by reading DR and SR
	uint8_t temp = SPI1->DR;
					temp = SPI1->SR;
}

void SPI_Receive (uint8_t *data, int size)
{
		/************** STEPS TO FOLLOW *****************
	1. Wait for the BSY bit to reset in Status Register
	2. Send some Dummy data before reading the DATA
	3. Wait for the RXNE bit to Set in the status Register
	4. Read data from Data Register
	************************************************/
	
		while (size)
		{
		while (((SPI1->SR)&(1<<7))) {;}  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
		SPI1->DR = 0;  // send dummy data
		while (!((SPI1->SR) &(1<<0))){;}  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
	  *data++ = (SPI1->DR);
		size--;
		}
}



