/*
 * ADC.c
 *
 *  Created on: 7 mar. 2023
 *      Author: Paul
 */
/* Project Headers */
#include "F28x_Project.h"

/* System Headefiles*/
#include <stdlib.h>

/* Own Headerfiles */
#include "SYSPARAM.h"
#include "ADC_A.h"
#include "ADC_B.h"
#include "ADC_C.h"
//#include "ADC_D.h"

/* Extern Headerfiles */

/* Function Prototypes */
void ADC_vADCModuleInit();
void ADC_vADCModuleInit()
{
    EALLOW;
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;//adcaint1
    //PieCtrlRegs.PIEIER1.bit.INTx2 = 1;//adcbint1
    //PieCtrlRegs.PIEIER1.bit.INTx3 = 1;//adccint1
    //PieCtrlRegs.PIEIER10.bit.INTx10 = 1;//adccint2
    //PieCtrlRegs.PIEIER10.bit.INTx11 = 1;//adccint3
    //PieCtrlRegs.PIEIER10.bit.INTx12 = 1;//adccint4
    //Group A ADCs
    ADC_A_vADCA_SOC0Config(stADCA1);
    ADC_A_vADCA_Group_Init();
    //Group B ADCs
    ADC_B_vADCB_SOC0Config(stADCB1);
    ADC_B_vADCB_Group_Init();
    //Group C ADCs
    ADC_C_vADCC_SOC3Config(stADCC1);
    ADC_C_vADCC_SOC0Config(stADCC2);
    ADC_C_vADCC_SOC1Config(stADCC3);
    ADC_C_vADCC_SOC2Config(stADCC4);
    ADC_C_vADCC_Group_Init();
}



