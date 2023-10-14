/*
 * ADC_B.c
 *
 *  Created on: 18 feb. 2023
 *      Author: ihate
 */
/* Project Headers */
#include "F28x_Project.h"

/* System Headefiles*/
#include <stdlib.h>
#include <ADC_B.h>
/* Own Headerfiles */
#include "SYSPARAM.h"
/* Extern Headerfiles */

/* Function Prototypes */

void ADC_B_vADCB_Group_Init();
void ADC_B_vADCB_SOC0Config(tstADCBConfig ADC);
void ADC_B_vADCB_SOC1Config(tstADCBConfig ADC);
Uint16 ADC_B_u16ADCBgetRawValue0();
Uint16 ADC_B_u16ADCBgetRawValue1();
interrupt void adcb1_isr(void);
interrupt void adcb2_isr(void);
interrupt void adcb3_isr(void);
interrupt void adcb4_isr(void);
/* Global Variables */
/*
    Hardware - Software Interface
    *48V Voltage Measurement - ADCINB4
    *12V Voltage Measurement  - ADCINB3
*/
#ifdef BUCK
tstADCBConfig stADCB1 = ADCB1_INIT_BUCK;
#endif

#ifdef BOOST
tstADCBConfig stADCB1 = ADCB1_INIT_BOOST;
#endif
tstADCBConfig stADCB2;
tstADCBConfig stADCB3;
tstADCBConfig stADCB4;

//Initalize ADC Group
void ADC_B_vADCB_Group_Init()
{
    EALLOW;
    AdcbRegs.ADCCTL2.bit.PRESCALE = 4;
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    // ADC_SIGNALMODE_SINGLE 0
    // ADC_SIGNALMODE_DIFFERENTIAL 1
    // ADC_RESOLUTION_12BIT
    //Set pulse positions to late
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;//0-impulsul pentru intrerupere la sfarsitul achzitiei,1--//-la sfarsitul conversiei
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    PieVectTable.ADCB1_INT = &adcb1_isr;
    PieVectTable.ADCB2_INT = &adcb2_isr;

    DELAY_US(1000);
    EDIS;
}
//Initialize ADC by SOC
void ADC_B_vADCB_SOC0Config(tstADCBConfig ADC)
{
        EALLOW;
        AdcbRegs.ADCSOC0CTL.bit.CHSEL = ADC.u16ADCChannel;  //pinul de unde citeste
        AdcbRegs.ADCSOC0CTL.bit.ACQPS = ADC.u16AqWindow; //sample window

        if(ADC.u16Interrupt==1)
        {
            //AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = ADC.u16SOCSEL; //end of SOC0 will set INT1 flag
            AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = EOC0; //end of SOC0 will set INT1 flag
            AdcbRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;   //enable INT1 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==2)
        {
            //AdcbRegs.ADCINTSEL1N2.bit.INT2SEL = ADC.u16SOCSEL; //end of SOC0 will set INT2 flag
            AdcbRegs.ADCINTSEL1N2.bit.INT2SEL = EOC0; //end of SOC0 will set INT2 flag
            AdcbRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;   //enable INT2 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT2 flag is cleared
        }
        if(ADC.u16Interrupt==3)
        {
            AdcbRegs.ADCINTSEL3N4.bit.INT3SEL = EOC0; //end of SOC0 will set INT3 flag
            //AdcaRegs.ADCINTSEL3N4.bit.INT3SEL = ADC.u16SOCSEL; //end of SOC0 will set INT3 flag
            AdcbRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;   //enable INT3 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT3 flag is cleared
        }
        if(ADC.u16Interrupt==4)
        {
            //AdcaRegs.ADCINTSEL3N4.bit.INT4SEL = ADC.u16SOCSEL; //end of SOC0 will set INT1 flag
            AdcbRegs.ADCINTSEL3N4.bit.INT4SEL = EOC0; //end of SOC0 will set INT4 flag
            AdcbRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT4 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT4 flag is cleared
        }
        AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = ADC.u16TriggerSRC;
        //AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
        EDIS;
}
//Initialize ADC by SOC
void ADC_B_vADCB_SOC1Config(tstADCBConfig ADC)
{
        EALLOW;
        AdcbRegs.ADCSOC1CTL.bit.CHSEL = ADC.u16ADCChannel;  //SOC will convert pin A0
        AdcbRegs.ADCSOC1CTL.bit.ACQPS = ADC.u16AqWindow; //sample window is 100 SYSCLK cycles

        if(ADC.u16Interrupt==1)
        {
            AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = EOC1; //end of SOC will set INT1 flag
            AdcbRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;  //enable INT1 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==2)
        {
            AdcbRegs.ADCINTSEL1N2.bit.INT2SEL = EOC1; //end of SOC0 will set INT1 flag
            AdcbRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;  //enable INT1 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==3)
        {
            AdcbRegs.ADCINTSEL3N4.bit.INT3SEL = EOC1; //end of SOC0 will set INT1 flag
            AdcbRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;;   //enable INT1 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==4)
        {
            AdcbRegs.ADCINTSEL3N4.bit.INT4SEL = EOC1; //end of SOC0 will set INT1 flag
            AdcbRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT1 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT1 flag is cleared
        }
        AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = ADC.u16TriggerSRC;
        //AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
        EDIS;
}

Uint16 ADC_B_u16ADCBgetRawValue0()
{
    return AdcbResultRegs.ADCRESULT0;
}

Uint16 ADC_B_u16ADCBgetRawValue1()
{
    return AdcbResultRegs.ADCRESULT1;
}

//ADCA Interrupt 1
interrupt void adcb1_isr(void)
{
    EALLOW;
    #ifdef DEBUG
    GpioDataRegs.GPASET.bit.GPIO10=1;
    #endif
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    #ifdef DEBUG
    GpioDataRegs.GPACLEAR.bit.GPIO10=1;
    #endif
    EDIS;
}
//ADCA Interrupt 2
interrupt void adcb2_isr(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear INT1 flag
}
//ADCA Interrupt 3
interrupt void adcb3_isr(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //clear INT1 flag
}
//ADCA Interrupt 4
interrupt void adcb4_isr(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //clear INT1 flag
}



