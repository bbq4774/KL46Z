#include "MKL46Z4.h"
#include "i2c.h"

typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} mag3110;

//Function Declarations
void init_MAG3110(void);
void Delay_mag(uint8_t);
mag3110 mag_read(void);
