#include "init.h"
#include "lcd.h"
#include "mag3110.h"
#include "math.h"

int main() {
	init_CLK();
	init_LED();
	init_SW();
	init_ITR_SW();
	init_SysTick_interrupt();
	init_LCD();
	init_I2C();
	init_MAG3110();
	
	reset_LED();
	
	while (1) {
		toggle_LED();
		mag3110 data = mag_read();
		float rad = atan2(data.y, data.x); //radian
		float deg = rad * (180.0 / 3.14159) ; //degre
		LCD_DisplayDecimal((uint16_t)deg);
	}
	return 0;
}
