#include "mag3110.h"

void Delay_mag(uint8_t time) {
	while (time != 0)
		time--;
}

void init_MAG3110() {
	I2C_write(0x01, 0x10);
	Delay_mag(1);
}

mag3110 mag_read() {
	mag3110 data;
	uint8_t x1= I2C_read(0x01);
	Delay_mag(1);
	uint8_t x2= I2C_read(0x02);
	Delay_mag(1);
	uint8_t y1= I2C_read(0x03);
	Delay_mag(1);
	uint8_t y2= I2C_read(0x04);
	Delay_mag(1);
	uint8_t z1= I2C_read(0x05);
	Delay_mag(1);
	uint8_t z2= I2C_read(0x06);
	Delay_mag(1);
	
	int16_t raw_x_data = ((int16_t)(x1 << 8 | x2));
  int16_t raw_y_data = ((int16_t)(y1 << 8 | y2));
  int16_t raw_z_data = ((int16_t)(z1 << 8 | z2));
	
	//Get data
	data.x = raw_x_data ;
  data.y = raw_y_data ;
	data.z = raw_z_data ;
	
	return data;
}
