#include "RccConfig.h"
#include "Delay.h"
#include "SPI.h"


float xg, yg, zg;
int16_t x,y,z;
uint8_t RxData[6];

void lis35de_write (uint8_t address, uint8_t value)
{

}

void lis35de_read (uint8_t address)
{

}

void lis35de_init (void)
{
	
}

int main ()
{
	SysClockConfig ();
	GPIOConfig ();
	SPIConfig ();
	
	SPI_Enable ();
	

	while(1)
	{
		
	}
}