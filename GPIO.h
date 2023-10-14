/*
 * GPIO.h
 *
 *  Created on: 15 feb. 2023
 *      Author: ihate
 */

#ifndef GPIO_H_
#define GPIO_H_



/*Defines*/
#define GPIO_MODE 0
#define EPWM_MODE 1
#define DIRECTION_IN 0
#define DIRECTION_OUT 1
#define PWM4MASK 0x000000C0
#define PWM5MASK 0x00000300
#define PWM6MASK 0x00000C00
/*Structures*/
/*Function prototypes*/
void GPIO_vConfig();
void GPIO_vConverterPrechargeModeConfig();
void GPIO_vConverterBalanceModeConfig();
void GPIO_vConverterOnModeConfig();
void GPIO_vConverterOffModeConfig();
void GPIO_vStrobeMOS1();
void GPIO_vStrobeMOS2();
void GPIO_vStrobeMOS3();
void GPIO_vStrobeMOS4();
void GPIO_vStrobeMOS1And3();
void GPIO_vStrobeMOS2And4();
/*Variables*/



#endif /* GPIO_H_ */
