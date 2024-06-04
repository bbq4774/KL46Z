#include "init.h"
#include "lcd.h"

int main() {
	init_CLK();
	init_LED();
	init_SW();
	init_ITR_SW();
	init_SysTick_interrupt();
	
	reset_LED();
	
	while (1) {
		toggle_LED();
		SegLCD_DisplayDecimal(123);
	}
	return 0;
}
