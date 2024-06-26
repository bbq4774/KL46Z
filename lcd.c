#include "lcd.h"

const static uint8_t LCD_Frontplane_Pin[LCD_NUM_FRONTPLANE_PINS] = {LCD_FRONTPLANE0, LCD_FRONTPLANE1, 
																																		LCD_FRONTPLANE2, LCD_FRONTPLANE3, 
																																		LCD_FRONTPLANE4, LCD_FRONTPLANE5, 
																																		LCD_FRONTPLANE6, LCD_FRONTPLANE7};
const static uint8_t LCD_Backplane_Pin[LCD_NUM_BACKPLANE_PINS] = {LCD_BACKPLANE0, LCD_BACKPLANE1, LCD_BACKPLANE2, LCD_BACKPLANE3};

void init_LCD(void){ //Initializes all components of SLCD on the FRDM-KL46Z
	LCD->GCR |= LCD_GCR_PADSAFE_MASK; //Set PADSAFE to disable LCD while configuring
	LCD->GCR &= ~LCD_GCR_LCDEN_MASK;  //Clear LCDEN (LCD Enable) while configuring
	
	//Configure pins *From Reference manual, set pins to MUX 0 for normal LCD display operation
	PORTB->PCR[7]  = PORT_PCR_MUX(0u);					//Set PTB7 to LCD_P7
	PORTB->PCR[8]  = PORT_PCR_MUX(0u);					//Set PTB8 to LCD_P8
	PORTB->PCR[10] = PORT_PCR_MUX(0u);					//Set PTB10 to LCD_P10
	PORTB->PCR[11] = PORT_PCR_MUX(0u);					//Set PTB11 to LCD_P11
	PORTB->PCR[21] = PORT_PCR_MUX(0u);					//Set PTB21 to LCD_P17
	PORTB->PCR[22] = PORT_PCR_MUX(0u);					//Set PTB22 to LCD_P18
	PORTB->PCR[23] = PORT_PCR_MUX(0u);					//Set PTB23 to LCD_P19
	PORTC->PCR[17] = PORT_PCR_MUX(0u);					//Set PTC17 to LCD_P37
	PORTC->PCR[18] = PORT_PCR_MUX(0u);					//Set PTC18 to LCD_P38
	PORTD->PCR[0]  = PORT_PCR_MUX(0u);					//Set PTD0 to LCD_P40
	PORTE->PCR[4]  = PORT_PCR_MUX(0u);					//Set PTE4 to LCD_P52
	PORTE->PCR[5]  = PORT_PCR_MUX(0u);					//Set PTE5 to LCD_P53
	
	//Configure LCD_GCR - General Control Register
	LCD->GCR =	LCD_GCR_CPSEL_MASK |						//Set LCD_GCR_CPSEL to use capacitor charge pump.
							LCD_GCR_LADJ(0x03) |						//Set LCD_GCR_LADJ to 11, slow clock rate = lower power
							LCD_GCR_PADSAFE_MASK |					//Set LCD_GCR_PADSAFE, leave enabled during configuration process.
							LCD_GCR_ALTDIV(0x03) |					//Set LCD_GCR_ALTDIV to 11, divide alternate clock by 512.  This is assuming an 8MHz External Crystal is used.
							LCD_GCR_FFR_MASK |							//Set LCD_GCR_FFR, allow an LCD Frame Frequency of 46.6Hz to 146.2Hz.
							LCD_GCR_SOURCE_MASK |						//Set LCD_GCR_SOURCE, Selects output of the alternate clock source selection as the LCD clock source.
							LCD_GCR_LCLK(0x07) | 						//Set LCD_GCR_LCLK to 111, LCD controller frame frequency
							LCD_GCR_DUTY(0x03);  						//Set LCD_GCR_DUTY to 011, Have 4 backplane pins, so need a 1/4 duty cycle.
	
	//Configure LCD_SEG_AR  - Auxiliary Register
	LCD->AR = LCD_AR_BRATE(0x00);	//Set LCD_SEG_AR_BRATE to 000. Frequency of blink is determined by LCD clock/(2^(12 + BRATE))
						
	//Configure LCD_SEG_FDCR - Fault Detect Control Register, controls use of Fault Detect features of SLCD.
	LCD->FDCR = 0x00000000; //Clear all bits in FDCR
	
	//Configure LCD_PENn - Pin Enable Register
	//LCD->PEN[0] contains bits 0-31, while LCD->PEN[1] contains bits 32-63.  For pins >= 32, set in LCD->PEN[1] as LCD_PEN_PEN(Pin#-32)
	LCD->PEN[0] =  LCD_PEN_PEN(1u<<7u) |				//LCD_P7
								 LCD_PEN_PEN(1u<<8u) |				//LCD_P8
								 LCD_PEN_PEN(1u<<10u)|				//LCD_P10
								 LCD_PEN_PEN(1u<<11u)|				//LCD_P11
								 LCD_PEN_PEN(1u<<17u)|				//LCD_P17
								 LCD_PEN_PEN(1u<<18u)|				//LCD_P18
								 LCD_PEN_PEN(1u<<19u);				//LCD_P19
	LCD->PEN[1] =  LCD_PEN_PEN(1u<<5u) |				//LCD_P37
								 LCD_PEN_PEN(1u<<6u) |				//LCD_P38
								 LCD_PEN_PEN(1u<<8u) |				//LCD_P40
								 LCD_PEN_PEN(1u<<20u)|				//LCD_P52
								 LCD_PEN_PEN(1u<<21u);				//LCD_P53
	
	//Configure LCD_SEG_BPENn - Back Plane Enable Register, controls which pins in LCD->PEN are Back Plane (commons)
	//LCD->BPEN[0] contains bits 0-31, while LCD->BPEN[1] contains bits 32-63.  For pins >= 32, set in LCD->BPEN[1] as LCD_BPEN_BPEN(Pin#-32)
	LCD->BPEN[0] = LCD_BPEN_BPEN(1u<<18u)|			//LCD_P18  COM3
								 LCD_BPEN_BPEN(1u<<19u);			//LCD_P19, COM2
	LCD->BPEN[1] = LCD_BPEN_BPEN(1u<<8u) |			//LCD_P40, COM0
								 LCD_BPEN_BPEN(1u<<20u);			//LCD_P52, COM1
	
	//Configure LCD_WFyTOx - Configures 4 Waveform signals
	//Where x is the n index value of WFn on the least significant byte (bits 7-0) and y is the n index value of WFn on the most significant byte (bits 31-24)
	LCD->WF[4]  = LCD_WF_WF18(0x8)|						//WF Pin 18 (COM3) is enabled on Phases D
								LCD_WF_WF19(0x4);						//WF Pin 19 (COM2) is enabled on Phases C
	LCD->WF[10] = LCD_WF_WF40(0x1);						//WF Pin 40 (COM0) is enabled on Phases A
	LCD->WF[13] = LCD_WF_WF52(0x2);					//WF Pin 52 (COM1) is enabled on Phases B
								
	//Disable GCR_PADSAFE and Enable GCR_LCDEN
	LCD->GCR &= ~LCD_GCR_PADSAFE_MASK; 					//Clear PADSAFE to unlock LCD pins
	LCD->GCR |= LCD_GCR_LCDEN_MASK;							//Set LCDEN to enable operation of LCD
}

void LCD_Set(uint8_t Value,uint8_t Digit){//Sets a value from 0-9
	if (Digit > 4)
	{
		LCD_DisplayError();
		return;
	}
	if(Value==0x00)			{LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E |LCD_SEG_F); 
												LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);}
	else if(Value==0x01){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_CLEAR); 
												LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_B | LCD_SEG_C);}
	else if(Value==0x02){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_G); 
												LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B);}
	else if(Value==0x03){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_G); 
												LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);}
	else if(Value==0x04){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_F | LCD_SEG_G); 
												LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_B | LCD_SEG_C);}
	else if(Value==0x05){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_F | LCD_SEG_G);
												LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_C);}
	else if(Value==0x06){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G); 
												LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_C);}
	else if(Value==0x07){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_CLEAR); 
												LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);}
	else if(Value==0x08){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G); 
												LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);}
	else if(Value==0x09){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_F | LCD_SEG_G); 
												LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);}
}

void LCD_DisplayDecimal(uint16_t Value){//Displays a 3 Digit number in decimal
	LCD_Set((Value - (Value/1000)*1000)/100,2);
	LCD_Set((Value - (Value/100)*100)/10,3);
	LCD_Set(Value - (Value/10)*10,4);
}

void LCD_DisplayError(){//Displays Err on screen
	LCD->WF8B[LCD_FRONTPLANE0] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G);
	LCD->WF8B[LCD_FRONTPLANE1] = (LCD_SEG_A);
	LCD->WF8B[LCD_FRONTPLANE2] = (LCD_SEG_E | LCD_SEG_G);
	LCD->WF8B[LCD_FRONTPLANE3] = (LCD_CLEAR);
	LCD->WF8B[LCD_FRONTPLANE4] = (LCD_SEG_E | LCD_SEG_G);
	LCD->WF8B[LCD_FRONTPLANE5] = (LCD_CLEAR);
	LCD->WF8B[LCD_FRONTPLANE6] = (LCD_CLEAR);
	LCD->WF8B[LCD_FRONTPLANE7] = (LCD_CLEAR);
}
