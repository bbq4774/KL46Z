#include "i2c.h"

void init_I2C(void) {
	//chon che do hoat dong PTE24 PTE25
	PORTE->PCR[25] |= PORT_PCR_MUX(5);	
	PORTE->PCR[24] |= PORT_PCR_MUX(5);	
	
	//Set pull up 
	PORTE->PCR[25] |= 1 << 1 | 1 << 0;
	PORTE->PCR[24] |= 1 << 1 | 1 << 0;
	
	//This enable I2C0
	I2C0->C1 |= 1 << 7;
}

void I2C_write(uint8_t data, uint8_t registerAddress) {
	I2C0->C1 |= I2C_C1_TX_MASK;		//Transmit Mode Select : Transmit
	I2C0->C1 |= I2C_C1_MST_MASK;	//Master Mode Select : When the MST bit is changed from a 0 to a 1, a START signal is generated 
	
	I2C0->D = 0x1C;	//Data transmit : 0x0E=>0001110 W(0) => 0x1C
	while(!(I2C0->S & I2C_S_IICIF_MASK));	//Interrupt Flag : 1 Interrupt pending
	I2C0->S |= I2C_S_IICIF_MASK; //Clear interrupt
	
	I2C0->D = registerAddress;	//Data transmit : registerAddress
	while(!(I2C0->S & I2C_S_IICIF_MASK)); //Interrupt Flag : 1 Interrupt pending
	I2C0->S |= I2C_S_IICIF_MASK; //Clear interrupt
	
	I2C0->D = data;	//Data transmit : data
	while(!(I2C0->S & I2C_S_IICIF_MASK)); //Interrupt Flag : 1 Interrupt pending
	I2C0->S |= I2C_S_IICIF_MASK; //Clear interrupt
	
	I2C0->C1 &= ~I2C_C1_MST_MASK;	//Master Mode Select : When this bit changes from a 1 to a 0, a STOP signal is generated
}

uint8_t I2C_read(uint8_t registerAddress) {
	uint8_t data;
	I2C0->C1 |= I2C_C1_TX_MASK;		//Transmit Mode Select : Transmit
	I2C0->C1 |= I2C_C1_MST_MASK;	//Master Mode Select : A START signal is generated 
	
	I2C0->D = 0x1C;	//Data transmit : 0x0E=>0001110 W(0) => 0x1C
	while(!(I2C0->S & I2C_S_IICIF_MASK));	//Interrupt Flag : 1 Interrupt pending
	I2C0->S |= I2C_S_IICIF_MASK; 	//Clear interrupt
	
	I2C0->D = registerAddress;		//Data transmit : registerAddress
	while(!(I2C0->S & I2C_S_IICIF_MASK));
	I2C0->S |= I2C_S_IICIF_MASK;
	
	I2C0->C1 |= I2C_C1_RSTA_MASK;	//Repeat START
	
	I2C0->D = 0x1D;	//Data transmit : 0x0E=>0001110 R(1) => 0x1D
	while(!(I2C0->S & I2C_S_IICIF_MASK));	//Interrupt Flag : 1 Interrupt pending
	I2C0->S |= I2C_S_IICIF_MASK; 	//Clear interrupt
	
	I2C0->C1 &= ~I2C_C1_TX_MASK;	//Transmit Mode Select : 0 Receive
	I2C0->C1 |= I2C_C1_TXAK_MASK;	//Transmit Acknowledge Enable : sent NAK
	
	data = I2C0->D;	//Receive data
	while(!(I2C0->S & I2C_S_IICIF_MASK));
	I2C0->S |= I2C_S_IICIF_MASK;
	
	I2C0->C1 &= ~I2C_C1_MST_MASK;	//Master Mode Select : A STOP signal is generated
	return data;
}
