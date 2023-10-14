/*
 * SYSCONFIG.c
 *
 *  Created on: 18 feb. 2023
 *      Author: ihate
 */


/* Project Headers */
#include "F28x_Project.h"

/* System Headefiles*/
#include <stdlib.h>
#include <SYSFUNCTIONS.h>
/* Own Headerfiles */
#include "SYSPARAM.h"
#include "NTC.h"
#include "ADC_C.h"
#include "PWM.h"
#include "GPIO.h"
#include "FILTER.h"
/* Extern Headerfiles */

/* Function Prototypes */
void SYSFUNCTIONS_vInitialConfig();
void SYSFUNCTIONS_vFinalConfig();
void SYSFUNCTIONS_vTemp1Calculate(tstTempMeasurements *TempMeasurements);
void SYSFUNCTIONS_vTemp2Calculate(tstTempMeasurements *TempMeasurements);
void SYSFUNCTIONS_vTemp3Calculate(tstTempMeasurements *TempMeasurements);
void SYSFUNCTIONS_vConverterStatus();
void SYSFUNCTIONS_vConverterUpdate();
void SYSFUNCTIONS_vMOSFETsStrobe();
void SYSFUNCTIONS_vPrechargeDutyRamp();

/* Global Variables */
tstTempMeasurements stTempMeasurements;
Uint16 u16ConverterStatus = CONVERTER_OFF;
Uint16 u16StrobeMOS1 = 0;
Uint16 u16StrobeMOS2 = 0;
Uint16 u16StrobeMOS3 = 0;
Uint16 u16StrobeMOS4 = 0;
Uint16 u16Update = 0;

void SYSFUNCTIONS_vInitialConfig()
{
    //init system clocks and get board speed running at 200 MHz
    InitSysCtrl();

    InitGpio();
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    EALLOW;

    IER |= M_INT1; //Enable group 1 interrupts
    IER |= M_INT3;
    IER |= M_INT10;
    IER |= M_INT11;//CLA
    IER |= M_INT13;//CPUTIMER1
    IER |= M_INT14;//CPUTIMER2

    EDIS;
}

void SYSFUNCTIONS_vFinalConfig()
{
    EALLOW;
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM
    EDIS;
}

void SYSFUNCTIONS_vTemp1Calculate(tstTempMeasurements *TempMeasurements)
{
        TempMeasurements->i16TempMeas1=NTC_i16GetTemperature( FILTER_u16Filter ( &stFILTER1, ADC_C_u16ADCCgetRawValue0() ) );
        if(TempMeasurements->i16TempMeas1 >= TEMPCRIT)
        {
            u16ConverterStatus = CONVERTER_OFF;
        }
}

void SYSFUNCTIONS_vTemp2Calculate(tstTempMeasurements *TempMeasurements)
{

        TempMeasurements->i16TempMeas2 = NTC_i16GetTemperature( FILTER_u16Filter ( &stFILTER2, ADC_C_u16ADCCgetRawValue1() ) );
        if(TempMeasurements->i16TempMeas2 >= TEMPCRIT)
        {
            u16ConverterStatus = CONVERTER_OFF;
        }
}

void SYSFUNCTIONS_vTemp3Calculate(tstTempMeasurements *TempMeasurements)
{
        TempMeasurements->i16TempMeas3=NTC_i16GetTemperature( FILTER_u16Filter ( &stFILTER3, ADC_C_u16ADCCgetRawValue2() ) );
        if(TempMeasurements->i16TempMeas3 >= TEMPCRIT)
        {
            u16ConverterStatus = CONVERTER_OFF;
        }
}

/*
void SYSFUNCTIONS_vMOSFETsStrobe()
{
    if(u16StrobeMOS1 == 1)
    {
        //for(int i = 0; i < 5; i++)
        //{
            //DELAY_US(10000);
            GPIO_vStrobeMOS1();
        //}
    u16StrobeMOS1 = 0;
    }

    if(u16StrobeMOS2 == 1)
    {
        //for(int i = 0; i < 5; i++)
        //{
            //DELAY_US(10000);
            GPIO_vStrobeMOS2();
        //}
    u16StrobeMOS2 = 0;
    }

    if(u16StrobeMOS3 == 1)
    {
        //for(int i = 0; i < 5; i++)
        //{
            //DELAY_US(10000);
            GPIO_vStrobeMOS3();
        //}
    u16StrobeMOS3 = 0;
    }

    if(u16StrobeMOS4 == 1)
    {
        //for(int i = 0; i < 5; i++)
        //{
            //DELAY_US(10000);
            GPIO_vStrobeMOS4();
        //}
    u16StrobeMOS4 = 0;
    }
}
*/

void SYSFUNCTIONS_vPrechargeDutyRamp()
{
    //if( stPWM3.u32Duty_percent < DUTYSTART )PWM_vEPWM6DutyUpdate(&stPWM3, stPWM3.u32Duty_percent + 5);
    if( stPWM1.u32Duty_percent < DUTYSTART )
    {
        PWM_vEPWM4DutyUpdate(&stPWM1, stPWM1.u32Duty_percent + 5);
        PWM_EPWM4GlobalLoad();
        DELAY_US(2);
    }
}

void SYSFUNCTIONS_vConverterStatus()
{
    switch( u16ConverterStatus )
    {

        case CONVERTER_OFF:
            GPIO_vConverterOffModeConfig();
            PWM_vEPWM4TripZoneStop();
            PWM_vEPWM5TripZoneStop();
        break;

        case CONVERTER_PRECHARGE:
            GPIO_vConverterPrechargeModeConfig();
            PWM_vEPWM4TripZoneClear();
            PWM_vEPWM5TripZoneClear();
            PWM_vEPWM4DutyUpdate(&stPWM1, DUTYMIN);
            PWM_EPWM4GlobalLoad();
        break;

        case CONVERTER_ON:
            GPIO_vConverterOnModeConfig();
            SYSFUNCTIONS_vPrechargeDutyRamp();
        break;

    }
}

void SYSFUNCTIONS_vConverterUpdate()
{
    if( u16Update == 1)
    {
        PWM_vEPWM4DeadbandUpdate( &stPWM1 );
        PWM_vEPWM5DeadbandUpdate( &stPWM2 );
    }
    u16Update = 0;
}
