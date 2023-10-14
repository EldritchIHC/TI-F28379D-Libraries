
#include <F28x_Project.h>
#include "SYSPARAM.h"
#include <SYSFUNCTIONS.h>
#include "CPU.h"
#include "PWM.h"
#include "GPIO.h"
#include "ADC.h"
#include "CLA.h"
#include "CLA_SHARED.h"
//#include "DAC.h"

int main(void)
{
/*
    Hardware - Software Interface
    *48V Voltage Measurement - ADCINB4
    *CMid Voltage Measurement - ADCINC4
    *12V Voltage Measurement  - ADCINB3
    *48V Current Measurement  - ADCINA4
    *12V Current Measurement - ADCINA3
    *Temp1 Temperature Measurement -ADCINC5
    *Temp2 Temperature Measurement - ADCIN15
    *Temp3 Temperature Measurement - ADCINC2
    *PWM1 - PWM5B
    *PWM2 - PWM5A
    *PWM3 - PWM4B
    *PWM4 - PWM4A
    *DACA - ADCA0
*/
    /*
     * o intrerupere cu 3 citiri curent,yensiune,cmid
     * o intrerupere pentru temperaturi
     * o intrerupere de cla ce modifica duty
     * o intrerupere de pwm ce actualizaeaza duty
     */
    EALLOW;
    SYSFUNCTIONS_vInitialConfig();
    //PWM
    PWM_vEPWMModuleInit();
    //PWM Muxing to GPIO
    GPIO_vConfig();
    //ADCs
    ADC_vADCModuleInit();
    //CPU1 Timer0 - Temperature sample frequency
    CPU_vCPUinit(stCPU1);
    //CPU1 Timer1 - Converter status check
    //CPU_vCPUinit(stCPU2);
    CLA_vModuleInit();
    //DAC
    //DAC_vDACInit();
    SYSFUNCTIONS_vFinalConfig();
    EDIS;
    while(1){
        SYSFUNCTIONS_vConverterStatus();
        SYSFUNCTIONS_vConverterUpdate();
        SYSFUNCTIONS_vTemp1Calculate(&stTempMeasurements);
        SYSFUNCTIONS_vTemp2Calculate(&stTempMeasurements);
        SYSFUNCTIONS_vTemp3Calculate(&stTempMeasurements);
        }
}

