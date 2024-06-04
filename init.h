#include "MKL46Z4.h"

//Define true false
#define true 1
#define false 0

//Creat variable extern
extern uint8_t volatile state;
extern uint32_t volatile msTicks;
extern uint32_t TICK_Green;
extern uint32_t TICK_Red;

//Function Declarations
void init_CLK(void);
void init_LED(void);
void reset_LED(void);
void init_SW(void);
void init_ITR_SW(void);
void PORTC_PORTD_IRQHandler(void);
void init_SysTick_interrupt(void);
void SysTick_Handler(void);
void Delay(uint32_t);
void toggle_LED(void);
