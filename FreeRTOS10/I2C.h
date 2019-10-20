/*
 * I2C.h
 *
 * Created: 18/10/2019 1:02:51
 *  Author: leoeg
 */ 

#include <avr/io.h>

#define F_CPU 16000000UL
#define false 0
#define true 1


#ifndef I2C_H_
#define I2C_H_

void startI2c();
char scanI2c (const uint8_t DEVICE);
void writeI2c (const uint8_t DEVICE, const uint8_t ADDRESS, const uint8_t DATA);
int16_t readI2c (const uint8_t DEVICE, const uint8_t ADDRESS);

#endif /* I2C_H_ */