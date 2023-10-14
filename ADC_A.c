/*
 * ADC_A.c
 *
 *  Created on: 29 aug. 2022
 *      Author: Paul
 */
/* Project Headers */
#include "F28x_Project.h"

/* System Headefiles*/
#include <stdlib.h>
#include <ADC_A.h>
#include <ADC_B.h>
#include <ADC_C.h>
#include "PWM.h"

/* Own Headerfiles */
#include "SYSPARAM.h"
/* Extern Headerfiles */

/* Function Prototypes */

void ADC_A_vADCA_Group_Init();
void ADC_A_vADCA_SOC0Config(tstADCAConfig ADC);
void ADC_A_vADCA_SOC1Config(tstADCAConfig ADC);
Uint16 ADC_A_u16ADCAgetRawValue0();
Uint16 ADC_A_u16ADCAgetRawValue1();
interrupt void adca1_isr(void);
interrupt void adca2_isr(void);
interrupt void adca3_isr(void);
interrupt void adca4_isr(void);
/* Global Variables */
//12V or 48V Current Measurement
/*Structure Initializations*/
/*
    Hardware - Software Interface
    *48V Current Measurement  - ADCINA4
    *12V Current Measurement - ADCINA3
*/
//12V Current Measurement
#ifdef BUCK
tstADCAConfig stADCA1 = ADCA1_INIT_BUCK;
#endif
//48V Current Measurement
#ifdef BOOST
tstADCAConfig stADCA1 = ADCA1_INIT_BOOST;
#endif

tstADCAConfig stADCA2;
tstADCAConfig stADCA3;
tstADCAConfig stADCA4;

float fVoltage12VMeasurement;
float fCurrent12VMeasurement;

float fVoltage48VMeasurement;
float fCurrent48VMeasurement;

float fCMidMeasurement;


//Initalize ADC Group
void ADC_A_vADCA_Group_Init()
{
    EALLOW;
    AdcaRegs.ADCCTL2.bit.PRESCALE = 4;
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    // ADC_SIGNALMODE_SINGLE 0
    // ADC_SIGNALMODE_DIFFERENTIAL 1
    // ADC_RESOLUTION_12BIT
    //Set pulse positions to late
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;//0-impulsul pentru intrerupere la sfarsitul achzitiei,1--//-la sfarsitul conversiei
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    PieVectTable.ADCA1_INT = &adca1_isr;
    PieVectTable.ADCA2_INT = &adca2_isr;

    DELAY_US(1000);
    EDIS;
}
//Initialize ADC by SOC
void ADC_A_vADCA_SOC0Config(tstADCAConfig ADC)
{
        EALLOW;
        AdcaRegs.ADCSOC0CTL.bit.CHSEL = ADC.u16ADCChannel;  //pinul de unde citeste
        AdcaRegs.ADCSOC0CTL.bit.ACQPS = ADC.u16AqWindow; //sample window

        if(ADC.u16Interrupt==1)
        {
            //AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = ADC.u16SOCSEL; //end of SOC0 will set INT1 flag
            AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = EOC0; //end of SOC0 will set INT1 flag
            AdcaRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;   //enable INT1 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==2)
        {
            //AdcaRegs.ADCINTSEL1N2.bit.INT2SEL = ADC.u16SOCSEL; //end of SOC0 will set INT2 flag
            AdcaRegs.ADCINTSEL1N2.bit.INT2SEL = EOC0; //end of SOC0 will set INT2 flag
            AdcaRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;   //enable INT2 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT2 flag is cleared
        }
        if(ADC.u16Interrupt==3)
        {
            AdcaRegs.ADCINTSEL3N4.bit.INT3SEL = EOC0; //end of SOC0 will set INT3 flag
            //AdcaRegs.ADCINTSEL3N4.bit.INT3SEL = ADC.u16SOCSEL; //end of SOC0 will set INT3 flag
            AdcaRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;   //enable INT3 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT3 flag is cleared
        }
        if(ADC.u16Interrupt==4)
        {
            //AdcaRegs.ADCINTSEL3N4.bit.INT4SEL = ADC.u16SOCSEL; //end of SOC0 will set INT1 flag
            AdcaRegs.ADCINTSEL3N4.bit.INT4SEL = EOC0; //end of SOC0 will set INT4 flag
            AdcaRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT4 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT4 flag is cleared
        }
        AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = ADC.u16TriggerSRC;
        //AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
        EDIS;
}
//Initialize ADC by SOC
void ADC_A_vADCA_SOC1Config(tstADCAConfig ADC)
{
        EALLOW;
        AdcaRegs.ADCSOC1CTL.bit.CHSEL = ADC.u16ADCChannel;  //SOC will convert pin A0
        AdcaRegs.ADCSOC1CTL.bit.ACQPS = ADC.u16AqWindow; //sample window is 100 SYSCLK cycles

        if(ADC.u16Interrupt==1)
        {
            AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = EOC1; //end of SOC will set INT1 flag
            AdcaRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;  //enable INT1 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==2)
        {
            AdcaRegs.ADCINTSEL1N2.bit.INT2SEL = EOC1; //end of SOC0 will set INT1 flag
            AdcaRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;  //enable INT1 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==3)
        {
            AdcaRegs.ADCINTSEL3N4.bit.INT3SEL = EOC1; //end of SOC0 will set INT1 flag
            AdcaRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;;   //enable INT1 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==4)
        {
            AdcaRegs.ADCINTSEL3N4.bit.INT4SEL = EOC1; //end of SOC0 will set INT1 flag
            AdcaRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT1 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT1 flag is cleared
        }
        AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = ADC.u16TriggerSRC;
        //AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
        EDIS;
}

Uint16 ADC_A_u16ADCAgetRawValue0()
{
    return AdcaResultRegs.ADCRESULT0;
}

Uint16 ADC_A_u16ADCAgetRawValue1()
{
    return AdcaResultRegs.ADCRESULT1;
}

//ADCA Interrupt 1
interrupt void adca1_isr(void)
{
    EALLOW;
    #ifdef DEBUG
    GpioDataRegs.GPASET.bit.GPIO10=1;
    #endif

    #ifdef BUCK
    //12V Current
    fVoltage12VMeasurement =  (  (float)ADC_B_u16ADCBgetRawValue0()  - VOLTAGE12V_OFFSET ) * VOLTAGE12V_SLOPE_INV ;
    //12V Voltage
    fCurrent12VMeasurement =  (  (float)ADC_A_u16ADCAgetRawValue0() - CURRENT12V_OFFSET ) * CURRENT12V_SLOPE_INV ;
    #endif

    #ifdef BOOST
    //12V Current
    fVoltage48VMeasurement =  (  (float)ADC_B_u16ADCBgetRawValue0() - VOLTAGE48V_OFFSET ) * VOLTAGE48V_SLOPE_INV ;
    //12V Voltage
    fCurrent48VMeasurement =  (  (float)ADC_A_u16ADCAgetRawValue0() - CURRENT48V_OFFSET ) * CURRENT48V_SLOPE_INV ;
    #endif
    //de pus in cla
    //CmidVoltage
    fCMidMeasurement =  (  (float)ADC_C_u16ADCCgetRawValue3()  - CMID_OFFSET ) * CMID_SLOPE_INV ;
    if( ( u16ConverterStatus == CONVERTER_PRECHARGE ) && ( fCMidMeasurement >= CMID_BUCK ) )
    {
        //u16ConverterStatus = CONVERTER_ON;
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    #ifdef DEBUG
    GpioDataRegs.GPACLEAR.bit.GPIO10=1;
    #endif
    EDIS;
}
//ADCA Interrupt 2
interrupt void adca2_isr(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear INT1 flag
}
//ADCA Interrupt 3
interrupt void adca3_isr(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //clear INT1 flag
}
//ADCA Interrupt 4
interrupt void adca4_isr(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //clear INT1 flag
}


