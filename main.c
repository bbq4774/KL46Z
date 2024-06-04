#include "init.h"
#include "lcd.h"
#include "mag3110.h"
#include "math.h"
#include "uart.h"

int main() {
	init_CLK();
	init_LED();
	init_SW();
	init_ITR_SW();
	init_SysTick_interrupt();
	init_LCD();
	init_I2C();
	init_MAG3110();
	UART_Init();
	
	reset_LED();
	
	while (1) {
		toggle_LED();
		mag3110 data = mag_read();
		float rad = atan2(data.y, data.x); //radian
		float deg = rad * (180.0 / 3.14159) ; //degree
		
		UART_SendString("   ___x:  ");
		UART_SendFloat(data.x);
		UART_SendString("   ___y:  ");
		UART_SendFloat(data.y);
		UART_SendString("   _goc:  ");
		UART_SendFloat(deg);
		UART_SendString("\n");
		
		LCD_DisplayDecimal((uint16_t)deg);
	}
	
	return 0;
}
