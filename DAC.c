/*
 * DAC.c
 *
 *  Created on: 6 mar. 2023
 *      Author: Paul
 */
#include "F28x_Project.h"

/* System Headefiles*/
#include <stdlib.h>

/* Own Headerfiles */
#include "SYSPARAM.h"
#include "DAC.h"

/* Extern Headerfiles */

/* Function Prototypes */
void DAC_vDACInit();
void DAC_vDACWriteValue(tstDACData DACData);
/* Global Variables */
tstDACData stDACData1;
tstDACData stDACData2;

void DAC_vDACInit()
{
    EALLOW;
    //DACA
    CpuSysRegs.PCLKCR16.bit.DAC_A = ENABLE;
    DacaRegs.DACCTL.bit.DACREFSEL = REFERENCE_VREF;
    DacaRegs.DACCTL.bit.SYNCSEL = EPWM4SYNCPER;
    DacaRegs.DACCTL.bit.LOADMODE = SYNCSELCLK;
    DacaRegs.DACOUTEN.bit.DACOUTEN = ENABLE;
    //DACB
    DacbRegs.DACCTL.bit.DACREFSEL = REFERENCE_VREF;
    DacbRegs.DACCTL.bit.SYNCSEL = EPWM4SYNCPER;
    DacbRegs.DACCTL.bit.LOADMODE = SYNCSELCLK;
    DacbRegs.DACOUTEN.bit.DACOUTEN = ENABLE;
    //DACC
    DaccRegs.DACCTL.bit.DACREFSEL = REFERENCE_VREF;
    DaccRegs.DACCTL.bit.SYNCSEL = EPWM4SYNCPER;
    DaccRegs.DACCTL.bit.LOADMODE = SYNCSELCLK;
    DaccRegs.DACOUTEN.bit.DACOUTEN = ENABLE;
    //DELAY_US(10);
    EDIS;
}
void DAC_vDACWriteValue(tstDACData DACData)
{
    if(DACData.u16DACChannel == CHANNEL_A)DacaRegs.DACVALS.bit.DACVALS = DACData.u16Value * DACData.fSlope + DACData.i16Offset;
    if(DACData.u16DACChannel == CHANNEL_B)DacbRegs.DACVALS.bit.DACVALS = DACData.u16Value * DACData.fSlope + DACData.i16Offset;
    if(DACData.u16DACChannel == CHANNEL_C)DaccRegs.DACVALS.bit.DACVALS = DACData.u16Value * DACData.fSlope + DACData.i16Offset;
}


