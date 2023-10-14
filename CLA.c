/*
 * CLA.c
 *
 *  Created on: 22 feb. 2023
 *      Author: Paul
 */
/* Project Headers */
#include "F28x_Project.h"

/* System Headefiles*/
#include <stdlib.h>
//#include <stdint.h>

/* Own Headerfiles */
#include "CLA.h"
#include "CLA_SHARED.h"
#include "SYSPARAM.h"

/* Extern Headerfiles */


/* Function Prototypes */
void CLA_vCLAMemoryConfig();
void CLA_vCpuClaInit();
void CLA_vRun();
void CLA_vModuleInit();
Uint16 CLA_u16TaskIsRunning();
__interrupt void cla1Isr1();
__interrupt void cla1Isr2();
__interrupt void cla1Isr3();
__interrupt void cla1Isr4();
__interrupt void cla1Isr5();
__interrupt void cla1Isr6();
__interrupt void cla1Isr7();
__interrupt void cla1Isr8();
/* Global Variables */
//To read from CPU
#pragma DATA_SECTION(fVoltageMeasurement,"CpuToCla1MsgRAM");
float fVoltageMeasurement;
//To read from CLA
#pragma DATA_SECTION(stCLAData1,"Cla1ToCpuMsgRAM");
tstCLAData stCLAData1;

void CLA_vCLAMemoryConfig()
{
    extern uint32_t Cla1funcsRunStart, Cla1funcsLoadStart, Cla1funcsLoadSize;
        EALLOW;

    #ifdef _FLASH
        //
        // Copy over code from FLASH to RAM
        //
        memcpy((uint32_t *)&Cla1funcsRunStart, (uint32_t *)&Cla1funcsLoadStart,
               (uint32_t)&Cla1funcsLoadSize);
    #endif //_FLASH

        //
        // Initialize and wait for CLA1ToCPUMsgRAM
        //
        MemCfgRegs.MSGxINIT.bit.INIT_CLA1TOCPU = 1;
        while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CLA1TOCPU != 1){};

        //
        // Initialize and wait for CPUToCLA1MsgRAM
        //
        MemCfgRegs.MSGxINIT.bit.INIT_CPUTOCLA1 = 1;
        while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CPUTOCLA1 != 1){};

        //
        // Select LS4RAM and LS5RAM to be the programming space for the CLA
        // First configure the CLA to be the master for LS4 and LS5 and then
        // set the space to be a program block
        //
        MemCfgRegs.LSxMSEL.bit.MSEL_LS4 = 1;
        MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS4 = 1;
        MemCfgRegs.LSxMSEL.bit.MSEL_LS5 = 1;
        MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS5 = 1;

        //
        // Next configure LS0RAM and LS1RAM as data spaces for the CLA
        // First configure the CLA to be the master for LS0(1) and then
        // set the spaces to be code blocks
        //
        MemCfgRegs.LSxMSEL.bit.MSEL_LS0 = 1;
        MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS0 = 0;

        MemCfgRegs.LSxMSEL.bit.MSEL_LS1 = 1;
        MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS1 = 0;

        EDIS;
}

void CLA_vCpuClaInit()
{
    //
    // Compute all CLA task vectors
    // On Type-1 CLAs the MVECT registers accept full 16-bit task addresses as
    // opposed to offsets used on older Type-0 CLAs
    //
    EALLOW;
    Cla1Regs.MVECT1 = (uint16_t)(&Cla1Task1);
    Cla1Regs.MVECT2 = (uint16_t)(&Cla1Task2);
    Cla1Regs.MVECT3 = (uint16_t)(&Cla1Task3);
    Cla1Regs.MVECT4 = (uint16_t)(&Cla1Task4);
    Cla1Regs.MVECT5 = (uint16_t)(&Cla1Task5);
    Cla1Regs.MVECT6 = (uint16_t)(&Cla1Task6);
    Cla1Regs.MVECT7 = (uint16_t)(&Cla1Task7);
    Cla1Regs.MVECT8 = (uint16_t)(&Cla1Task8);

    //
    // Enable the IACK instruction to start a task on CLA in software
    // for all  8 CLA tasks. Also, globally enable all 8 tasks (or a
    // subset of tasks) by writing to their respective bits in the
    // MIER register
    //
    DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK1 = CLA_TRIG_ADCAINT1;
    Cla1Regs.MCTL.bit.IACKE = 1;
    Cla1Regs.MIER.bit.INT1 = ENABLE;

    //
    // Configure the vectors for the end-of-task interrupt for all
    // 8 tasks
    //
    PieVectTable.CLA1_1_INT = &cla1Isr1;
    PieVectTable.CLA1_2_INT = &cla1Isr2;
    PieVectTable.CLA1_3_INT = &cla1Isr3;
    PieVectTable.CLA1_4_INT = &cla1Isr4;
    PieVectTable.CLA1_5_INT = &cla1Isr5;
    PieVectTable.CLA1_6_INT = &cla1Isr6;
    PieVectTable.CLA1_7_INT = &cla1Isr7;
    PieVectTable.CLA1_8_INT = &cla1Isr8;

}

void CLA_vRun()
{
    EALLOW;

    EDIS;
}

void CLA_vModuleInit()
{
    EALLOW;
    PieCtrlRegs.PIEIER11.bit.INTx1 = 1;//cla1_task1int
    CLA_vCLAMemoryConfig();
    CLA_vCpuClaInit();
}

Uint16 CLA_u16Task1IsRunning()
{
    return Cla1Regs.MIRUN.bit.INT1;
}

__interrupt void cla1Isr1 ()
{
    EALLOW;
    #ifdef DEBUG
    GpioDataRegs.GPASET.bit.GPIO10=1;
    #endif
    PieCtrlRegs.PIEACK.all = M_INT11;

    //PWM_vEPWM4DutyUpdate(&stPWM1, (Uint16)stCLAData1.fDuty[4] );
    //PWM_vEPWM5DutyUpdate(&stPWM2, (Uint16)stCLAData1.fDuty[4] );

    #ifdef DEBUG
    GpioDataRegs.GPACLEAR.bit.GPIO10=1;
    #endif
    EDIS;

}

__interrupt void cla1Isr2 ()
{
    PieCtrlRegs.PIEACK.all = M_INT11;
}

__interrupt void cla1Isr3 ()
{
    PieCtrlRegs.PIEACK.all = M_INT11;
}


__interrupt void cla1Isr4 ()
{
    PieCtrlRegs.PIEACK.all = M_INT11;
}


__interrupt void cla1Isr5 ()
{
    PieCtrlRegs.PIEACK.all = M_INT11;
}


__interrupt void cla1Isr6 ()
{
    PieCtrlRegs.PIEACK.all = M_INT11;
}

__interrupt void cla1Isr7 ()
{
    PieCtrlRegs.PIEACK.all = M_INT11;
}

__interrupt void cla1Isr8 ()
{
    PieCtrlRegs.PIEACK.all = M_INT11;
}
