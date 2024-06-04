#include "init.h"

uint8_t volatile state = false;
uint32_t volatile msTicks = 0;
uint32_t TICK_Green = 1000; // 1000ms = 1Hz
uint32_t TICK_Red = 500; // 500ms = 2Hz

//Init CLK System
void init_CLK() {
	SIM->SCGC4 |= 1 << 6;  //This enables the clock I2C0
	SIM->SCGC5 |= 1 << 10; //This enables the clock to PORTB
	SIM->SCGC5 |= 1 << 11; //This enables the clock to PORTC
	SIM->SCGC5 |= 1 << 12; //This enables the clock to PortD
	SIM->SCGC5 |= 1 << 13; //This enables the clock to PortE
	SIM->SCGC5 |= 1 << 19; //This enables the clock to Segment LCD Clock Gate Control
}

//Init LED Gree - Red
void init_LED() {
	//LED Green PortD5
	PORTD->PCR[5] |= 1 << 8; //Mux 001 | GPIO
	PTD->PDDR |= 1 << 5; //This sets PTD5 as an output
	
	//LED Red PortE29
	PORTE->PCR[29] |= 1 << 8; //Mux 001 | GPIO
	PTE->PDDR |= 1 << 29; //This sets PTE29 as an output
}

void reset_LED() {//When press SW2 (reset)
	PTD->PSOR |= 1 << 5;
	PTE->PCOR |= 1 << 29;
}

//Init SW
void init_SW() {
	//SW1 PORTC3
  PORTC->PCR[3] |= 1u << 8 | 1 << 1 | 1 << 0; //This sets GPIO and Enable pull-up resistor
  PTC->PDDR &= ~(1u << 3); //This sets PTC3 as an input. 
}

//Init SW interupt
void init_ITR_SW(void) {
	//Select falling edge interrupts for PORTC[3].
	PORTC->PCR[3]	|=  (0xA) << 16; //OR		PORTC->PCR[3] |= PORT_PCR_IRQC(0xA);
	
	NVIC_ClearPendingIRQ(31); // number for both PORTC and PORTD are 31
	NVIC_EnableIRQ(31);
}

//Init SysTick interupt
void init_SysTick_interrupt() {
	SysTick->LOAD = SystemCoreClock / 1000; //configured the SysTick to count in 1ms
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk 
								| SysTick_CTRL_TICKINT_Msk
								| SysTick_CTRL_ENABLE_Msk;
}

//SW1 interrupt Handler
void PORTC_PORTD_IRQHandler(void) {
	//If SW1 not press => do nothing
	if ((PTC->PDIR & (1<<3)) != 0)
		return;
	
	//Toggle state System
	if (state == false) {
		PTD->PCOR |= 1 << 5;
		PTE->PSOR |= 1 << 29;
		state = true;
	}
	else {
		PTD->PSOR |= 1 << 5;
		PTE->PCOR |= 1 << 29;
		state = false;
	}

	/* Clear interrupt service flag */
	PORTC->PCR[3] |= 1 << 24; 
}

//Blink LED
void toggle_LED() {
	if (state == false) {	
			PTE->PTOR |= 1 << 29;
			msTicks = 0;
			Delay(TICK_Red);
		} 
		else {
			PTD->PTOR |= 1 << 5;
			msTicks = 0;
			Delay(TICK_Green);
		}
}

void Delay(uint32_t TICK) {
	while (msTicks < TICK) {}; // Wait 500ms
	msTicks = 0; // Reset counter
}

// SysTick interrupt Handler
void SysTick_Handler() { 
	msTicks++; // Increment counter
}
