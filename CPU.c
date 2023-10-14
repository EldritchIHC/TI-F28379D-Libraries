/*
 * CPU.c
 *
 *  Created on: 30 aug. 2022
 *      Author: Paul
 */
/* Project Headers */
#include "F28x_Project.h"

/* System Headefiles*/
#include <stdlib.h>

/* Own Headerfiles */
#include "CPU.h"
#include "SYSPARAM.h"
#include "GPIO.h"
#include "SYSFUNCTIONS.h"

/* Extern Headerfiles */


/* Function Prototypes */
void CPU_vCPUinit(tstCPUConfig CPU);
interrupt void cputimer0_isr();
interrupt void cputimer1_isr();
interrupt void cputimer2_isr();

/* Global Variables */
tstCPUConfig stCPU1 = CPU1_INIT;
tstCPUConfig stCPU2 = CPU2_INIT;
tstCPUConfig stCPU3 = CPU3_INIT;
//CPU Timer Initialization
//Begin
void CPU_vCPUinit(tstCPUConfig CPU)
{
    EALLOW;
    Uint64 u64TBPRD;
    Uint16 u16PRESCALER=0;
    Uint16 u16LPRESCALER=0;//low bits of divider
    Uint16 u16HPRESCALER=0;//high bits of divider
    u64TBPRD = (CPU.u32Period_ms*MS_TO_NS)/SYS_PERIOD_NS;
    while(u64TBPRD > UINT32MAX)
    {
         if(u16PRESCALER == 0)
             u16PRESCALER = 1;
         else
             u16PRESCALER = u16PRESCALER + 1;
         u64TBPRD = ( CPU.u32Period_ms * MS_TO_NS ) / ( SYS_PERIOD_NS * u16PRESCALER );
    }

    u16LPRESCALER=u16PRESCALER & 0x00FF;
    u16HPRESCALER=(u16PRESCALER & 0xFF00)>>8;

    switch(CPU.u16CPUTimer)
        {
            case CPUTIMER0:
                CpuTimer0Regs.TCR.bit.TSS = 1; // Stop Timer
                CpuTimer0Regs.PRD.all = u64TBPRD;     // Set the Period Register
                CpuTimer0Regs.TPR.bit.TDDR = u16LPRESCALER; // Set prescaler registers to 0
                CpuTimer0Regs.TPRH.bit.TDDRH =u16HPRESCALER ; // Set prescaler high registers to 0
                CpuTimer0Regs.TCR.bit.TRB = 1; // Reload Timer
                CpuTimer0Regs.TCR.bit.SOFT = 1;
                CpuTimer0Regs.TCR.bit.FREE = 1;
                CpuTimer0Regs.TCR.bit.TIF = 1; // Clear Interrupt Flag
                CpuTimer0Regs.TCR.bit.TIE = CPU.u16CPUTimerInterruptEnable; // Enable/Disable Timer Interrupts
                CpuTimer0Regs.TCR.bit.TSS = 0; // Start Timer
            break;

            case CPUTIMER1:
                CpuTimer1Regs.TCR.bit.TSS = 1; // Stop Timer
                CpuTimer1Regs.PRD.all = u64TBPRD;     // Set the Period Register
                CpuTimer1Regs.TPR.bit.TDDR = u16LPRESCALER; // Set prescaler registers to 0
                CpuTimer1Regs.TPRH.bit.TDDRH = u16HPRESCALER; // Set prescaler high registers to 0
                CpuTimer1Regs.TCR.bit.TRB = 1; // Reload Timer
                CpuTimer1Regs.TCR.bit.SOFT = 1;
                CpuTimer1Regs.TCR.bit.FREE = 1;
                CpuTimer1Regs.TCR.bit.TIF = 1; // Clear Interrupt Flag
                CpuTimer1Regs.TCR.bit.TIE = CPU.u16CPUTimerInterruptEnable; // Disable Timer Interrupts
                CpuTimer1Regs.TCR.bit.TSS = 0; // Start Timer
            break;

            case CPUTIMER2:
                CpuTimer2Regs.TCR.bit.TSS = 1; // Stop Timer
                CpuTimer2Regs.PRD.all = u64TBPRD;     // Set the Period Register
                CpuTimer2Regs.TPR.bit.TDDR= u16LPRESCALER; // Set prescaler registers to 0
                CpuTimer2Regs.TPRH.bit.TDDRH = u16HPRESCALER; // Set prescaler high registers to 0
                CpuTimer2Regs.TCR.bit.TRB = 1; // Reload Timer
                CpuTimer2Regs.TCR.bit.SOFT = 1;
                CpuTimer2Regs.TCR.bit.FREE = 1;
                CpuTimer2Regs.TCR.bit.TIF = 1; // Clear Interrupt Flag
                CpuTimer2Regs.TCR.bit.TIE = CPU.u16CPUTimerInterruptEnable; // Disable Timer Interrupts
                CpuTimer2Regs.TCR.bit.TSS = 0; // Start Timer
            break;


        }
    //PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    //PieVectTable.TIMER0_INT = &cputimer0_isr;

    PieVectTable.TIMER1_INT = &cputimer1_isr;
   // PieVectTable.TIMER2_INT = &cputimer2_isr;
    EDIS;
}
//End CPU Timer Initialization
__interrupt void cputimer0_isr(void)
{
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; // Acknowledge Interrupt Group
   CpuTimer0Regs.TCR.bit.TIF = 1; // Clear Interrupt Flag

}

__interrupt void cputimer1_isr(void)
{
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; // Acknowledge Interrupt Group
   CpuTimer1Regs.TCR.bit.TIF = 1; // Clear Interrupt Flag
   //SYSFUNCTIONS_vConverterStatus();
}
