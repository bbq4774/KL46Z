#include "MKL46Z4.h"

//Function Declarations
void init_LCD(void);
void LCD_Set(uint8_t Value,uint8_t Digit);
void LCD_DisplayDecimal(uint16_t Value);
void LCD_DisplayError(void);

//Define Number of Front and Back plane pins
#define LCD_NUM_FRONTPLANE_PINS 8
#define LCD_NUM_BACKPLANE_PINS 4

//Create macros for segments
#define LCD_SEG_D 0x1
#define LCD_SEG_E 0x2
#define LCD_SEG_G 0x4
#define LCD_SEG_F 0x8
#define LCD_SEG_C 0x2
#define LCD_SEG_B 0x4
#define LCD_SEG_A 0x8
#define LCD_CLEAR 0x0

//Create Macros for each pin
#define LCD_FRONTPLANE0 37u
#define LCD_FRONTPLANE1 17u
#define LCD_FRONTPLANE2 7u
#define LCD_FRONTPLANE3 8u
#define LCD_FRONTPLANE4 53u
#define LCD_FRONTPLANE5 38u
#define LCD_FRONTPLANE6 10u
#define LCD_FRONTPLANE7 11u
#define LCD_BACKPLANE0 40u
#define LCD_BACKPLANE1 52u
#define LCD_BACKPLANE2 19u
#define LCD_BACKPLANE3 18u
