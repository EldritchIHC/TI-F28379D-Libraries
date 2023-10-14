/*
 * CLA.h
 *
 *  Created on: 22 feb. 2023
 *      Author: ihate
 */

#ifndef CLA_H_
#define CLA_H_


/*Defines*/

/*Structures*/

/*Function prototypes*/
void CLA_vCLAMemoryConfig();
void CLA_vCpuClaInit();
void CLA_vRun();
void CLA_vCpuClaInit();
void CLA_vModuleInit();
Uint16 CLA_u16Task1IsRunning();
__interrupt void cla1Isr1();
__interrupt void cla1Isr2();
__interrupt void cla1Isr3();
__interrupt void cla1Isr4();
__interrupt void cla1Isr5();
__interrupt void cla1Isr6();
__interrupt void cla1Isr7();
__interrupt void cla1Isr8();
/*Variables*/


#endif /* CLA_H_ */
