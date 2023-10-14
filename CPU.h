/*
 * CPU.h
 *
 *  Created on: 12 feb. 2023
 *      Author: Paul
 */

#ifndef CPU_H_
#define CPU_H_

/*Defines*/
#define CPUTIMER0 0
#define CPUTIMER1 1
#define CPUTIMER2 2

/*Structures*/
typedef struct{
    Uint32 u32Period_ms;
    Uint16 u16CPUTimer;
    Uint16 u16CPUTimerInterruptEnable;
}tstCPUConfig;
/*Structure Initializations*/
//TIMER0
#define CPU1_INIT {  50, CPUTIMER0, ENABLE }
//TIMER1
#define CPU2_INIT {    1, CPUTIMER1, ENABLE }
//TIMER2
#define CPU3_INIT {100000, CPUTIMER2, DISABLE}
/*Function prototypes*/
void CPU_vCPUinit(tstCPUConfig CPU);

/*Variables*/
extern tstCPUConfig stCPU1;
extern tstCPUConfig stCPU2;
extern tstCPUConfig stCPU3;
#endif /* CPU_H_ */
