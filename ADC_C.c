/*
 * ADC_C.c
 *
 *  Created on: 18 feb. 2023
 *      Author: ihate
 */
/* Project Headers */
#include "F28x_Project.h"

/* System Headefiles*/
#include <stdlib.h>
#include <ADC_C.h>
/* Own Headerfiles */
#include "SYSPARAM.h"


/* Extern Headerfiles */

/* Function Prototypes */

void ADC_C_vADCC_Group_Init();
void ADC_C_vADCC_SOC0Config(tstADCCConfig ADC);
void ADC_C_vADCC_SOC1Config(tstADCCConfig ADC);
void ADC_C_vADCC_SOC2Config(tstADCCConfig ADC);
void ADC_C_vADCC_SOC3Config(tstADCCConfig ADC);
void ADC_C_vADCC_SOC0Disable();
void ADC_C_vADCC_SOC1Disable();
void ADC_C_vADCC_SOC2Disable();
void ADC_C_vADCC_SOC3Disable();
Uint16 ADC_C_u16ADCCgetRawValue0();
Uint16 ADC_C_u16ADCCgetRawValue1();
interrupt void adcc1_isr(void);
interrupt void adcc2_isr(void);
interrupt void adcc3_isr(void);
interrupt void adcc4_isr(void);
/* Global Variables */
//CMID Voltage Measurement
tstADCCConfig stADCC1 = ADCC1_INIT;
//Temp1
tstADCCConfig stADCC2 = ADCC2_INIT;
//Temp2
tstADCCConfig stADCC3 = ADCC3_INIT;
//Temp3
tstADCCConfig stADCC4 = ADCC4_INIT;


//Initalize ADC Group
void ADC_C_vADCC_Group_Init()
{
    EALLOW;
    AdccRegs.ADCCTL2.bit.PRESCALE = 4;
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    // ADC_SIGNALMODE_SINGLE 0
    // ADC_SIGNALMODE_DIFFERENTIAL 1
    // ADC_RESOLUTION_12BIT
    //Set pulse positions to late
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;//0-impulsul pentru intrerupere la sfarsitul achzitiei,1--//-la sfarsitul conversiei
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    PieVectTable.ADCC1_INT = &adcc1_isr;
    PieVectTable.ADCC2_INT = &adcc2_isr;
    PieVectTable.ADCC3_INT = &adcc3_isr;
    PieVectTable.ADCC4_INT = &adcc4_isr;

    DELAY_US(1000);
    EDIS;
}
//Initialize ADC by SOC
void ADC_C_vADCC_SOC0Config(tstADCCConfig ADC)
{
        EALLOW;
        AdccRegs.ADCSOC0CTL.bit.CHSEL = ADC.u16ADCChannel;  //pinul de unde citeste
        AdccRegs.ADCSOC0CTL.bit.ACQPS = ADC.u16AqWindow; //sample window

        if(ADC.u16Interrupt==1)
        {
            //AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = ADC.u16SOCSEL; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL1N2.bit.INT1SEL = EOC0; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;   //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==2)
        {
            //AdcbRegs.ADCINTSEL1N2.bit.INT2SEL = ADC.u16SOCSEL; //end of SOC0 will set INT2 flag
            AdccRegs.ADCINTSEL1N2.bit.INT2SEL = EOC0; //end of SOC0 will set INT2 flag
            AdccRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;   //enable INT2 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT2 flag is cleared
        }
        if(ADC.u16Interrupt==3)
        {
            AdccRegs.ADCINTSEL3N4.bit.INT3SEL = EOC0; //end of SOC0 will set INT3 flag
            //AdcaRegs.ADCINTSEL3N4.bit.INT3SEL = ADC.u16SOCSEL; //end of SOC0 will set INT3 flag
            AdccRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;   //enable INT3 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT3 flag is cleared
        }
        if(ADC.u16Interrupt==4)
        {
            //AdcaRegs.ADCINTSEL3N4.bit.INT4SEL = ADC.u16SOCSEL; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL3N4.bit.INT4SEL = EOC0; //end of SOC0 will set INT4 flag
            AdccRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT4 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT4 flag is cleared
        }
        AdccRegs.ADCSOC0CTL.bit.TRIGSEL = ADC.u16TriggerSRC;
        //AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
        EDIS;
}

//Disable the Trigger of the SOC
void ADC_C_vADCC_SOC0Disable()
{
    EALLOW;
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = DISABLE;//0x00 - Software only trigger
    EDIS;
}

//Initialize ADC by SOC
void ADC_C_vADCC_SOC1Config(tstADCCConfig ADC)
{
        EALLOW;
        AdccRegs.ADCSOC1CTL.bit.CHSEL = ADC.u16ADCChannel;  //SOC will convert pin A0
        AdccRegs.ADCSOC1CTL.bit.ACQPS = ADC.u16AqWindow; //sample window is 100 SYSCLK cycles

        if(ADC.u16Interrupt==1)
        {
            AdccRegs.ADCINTSEL1N2.bit.INT1SEL = EOC1; //end of SOC will set INT1 flag
            AdccRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;  //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==2)
        {
            AdccRegs.ADCINTSEL1N2.bit.INT2SEL = EOC1; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;  //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==3)
        {
            AdccRegs.ADCINTSEL3N4.bit.INT3SEL = EOC1; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;;   //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==4)
        {
            AdccRegs.ADCINTSEL3N4.bit.INT4SEL = EOC1; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT1 flag is cleared
        }
        AdccRegs.ADCSOC1CTL.bit.TRIGSEL = ADC.u16TriggerSRC;
        //AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
        EDIS;
}

//Disable the Trigger of the SOC
void ADC_C_vADCC_SOC1Disable()
{
    EALLOW;
    AdccRegs.ADCSOC1CTL.bit.TRIGSEL = DISABLE;//0x00 - Software only trigger
    EDIS;
}

//Initialize ADC by SOC
void ADC_C_vADCC_SOC2Config(tstADCCConfig ADC)
{
        EALLOW;
        AdccRegs.ADCSOC2CTL.bit.CHSEL = ADC.u16ADCChannel;  //SOC will convert pin A0
        AdccRegs.ADCSOC2CTL.bit.ACQPS = ADC.u16AqWindow; //sample window is 100 SYSCLK cycles

        if(ADC.u16Interrupt==1)
        {
            AdccRegs.ADCINTSEL1N2.bit.INT1SEL = EOC2; //end of SOC will set INT1 flag
            AdccRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;  //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==2)
        {
            AdccRegs.ADCINTSEL1N2.bit.INT2SEL = EOC2; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;  //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==3)
        {
            AdccRegs.ADCINTSEL3N4.bit.INT3SEL = EOC2; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;;   //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==4)
        {
            AdccRegs.ADCINTSEL3N4.bit.INT4SEL = EOC2; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT1 flag is cleared
        }
        AdccRegs.ADCSOC2CTL.bit.TRIGSEL = ADC.u16TriggerSRC;
        //AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
        EDIS;
}

//Disable the Trigger of the SOC
void ADC_C_vADCC_SOC2Disable()
{
    EALLOW;
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL = DISABLE;//0x00 - Software only trigger
    EDIS;
}

//Initialize ADC by SOC
void ADC_C_vADCC_SOC3Config(tstADCCConfig ADC)
{
        EALLOW;
        AdccRegs.ADCSOC3CTL.bit.CHSEL = ADC.u16ADCChannel;  //SOC will convert pin A0
        AdccRegs.ADCSOC3CTL.bit.ACQPS = ADC.u16AqWindow; //sample window is 100 SYSCLK cycles

        if(ADC.u16Interrupt==1)
        {
            AdccRegs.ADCINTSEL1N2.bit.INT1SEL = EOC3; //end of SOC will set INT1 flag
            AdccRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;  //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==2)
        {
            AdccRegs.ADCINTSEL1N2.bit.INT2SEL = EOC3; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;  //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==3)
        {
            AdccRegs.ADCINTSEL3N4.bit.INT3SEL = EOC3; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;;   //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==4)
        {
            AdccRegs.ADCINTSEL3N4.bit.INT4SEL = EOC3; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT1 flag is cleared
        }
        AdccRegs.ADCSOC3CTL.bit.TRIGSEL = ADC.u16TriggerSRC;
        //AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
        EDIS;
}

//Disable the Trigger of the SOC
void ADC_C_vADCC_SOC3Disable()
{
    EALLOW;
    AdccRegs.ADCSOC3CTL.bit.TRIGSEL = DISABLE;//0x00 - Software only trigger
    EDIS;
}

Uint16 ADC_C_u16ADCCgetRawValue0()
{
    return AdccResultRegs.ADCRESULT0;
}

Uint16 ADC_C_u16ADCCgetRawValue1()
{
    return AdccResultRegs.ADCRESULT1;
}

Uint16 ADC_C_u16ADCCgetRawValue2()
{
    return AdccResultRegs.ADCRESULT2;
}

Uint16 ADC_C_u16ADCCgetRawValue3()
{
    return AdccResultRegs.ADCRESULT3;
}

//ADCA Interrupt 1
interrupt void adcc1_isr(void)
{
    EALLOW;
    #ifdef DEBUG
    GpioDataRegs.GPASET.bit.GPIO10=1;
    #endif
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    #ifdef DEBUG
    GpioDataRegs.GPACLEAR.bit.GPIO10=1;
    #endif
    EDIS;
}
//ADCA Interrupt 2
interrupt void adcc2_isr(void)
{
    EALLOW;
    #ifdef DEBUG
    GpioDataRegs.GPASET.bit.GPIO10=1;
    #endif
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
    AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear INT2 flag
    #ifdef DEBUG
    GpioDataRegs.GPACLEAR.bit.GPIO10=1;
    #endif
    EDIS;
}
//ADCA Interrupt 3
interrupt void adcc3_isr(void)
{
    EALLOW;
    #ifdef DEBUG
    GpioDataRegs.GPASET.bit.GPIO10=1;
    #endif
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
    AdccRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //clear INT3 flag
    #ifdef DEBUG
    GpioDataRegs.GPACLEAR.bit.GPIO10=1;
    #endif
    EDIS;
}
//ADCA Interrupt 4
interrupt void adcc4_isr(void)
{
    EALLOW;
    #ifdef DEBUG
    GpioDataRegs.GPASET.bit.GPIO10=1;
    #endif
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
    AdccRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //clear INT4 flag
    #ifdef DEBUG
    GpioDataRegs.GPACLEAR.bit.GPIO10=1;
    #endif
    EDIS;
}



