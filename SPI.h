#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "RccConfig.h"

void SPIConfig (void);

void GPIOConfig (void);

void SPI_Enable (void);

void SPI_Disable (void);

void CS_Enable (void);

void CS_Disable (void);

void SPI_Transmit (uint8_t *data, int size);

void SPI_Receive (uint8_t *data, int size);

#endif