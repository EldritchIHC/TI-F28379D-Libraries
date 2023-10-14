/*
 * ADC_C.h
 *
 *  Created on: 18 feb. 2023
 *      Author: ihate
 */

#ifndef ADC_C_H_
#define ADC_C_H_


/*Defines*/
/*CPU Timer Interrupt must be enabled*/
#define CPU1T0 1
#define CPU1T1 2
#define CPU1T2 3

#define EPWM1SOCA 5
#define EPWM1SOCB 6
#define EPWM2SOCA 2
#define EPWM2SOCB 8
#define EPWM4SOCA 11
#define EPWM4SOCB 12
#define EPWM5SOCA 13
#define EPWM5SOCB 14

#define EOC0 0
#define EOC1 1
#define EOC2 2
#define EOC3 3
/*Structures*/
typedef struct{
    Uint16 u16ADCChannel;//Input Pin
    Uint16 u16AqWindow;//
    Uint16 u16Interrupt;//Interrupt number 1 - 4
    Uint16 u16TriggerSRC;//Trigger Source(EPWM/CPU)
}tstADCCConfig;

/*Structure Initializations*/
//CMID Voltage Measurement
#define ADCC1_INIT {4, 14, 1, EPWM4SOCA}
//Temp1
#define ADCC2_INIT {2, 14, 2, CPU1T0}
//Temp2
#define ADCC3_INIT {5, 14, 3, CPU1T0}
//Temp3
#define ADCC4_INIT {15, 14, 4, CPU1T0}

/*Function prototypes*/
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
Uint16 ADC_C_u16ADCCgetRawValue2();
Uint16 ADC_C_u16ADCCgetRawValue3();
/*Variables*/
extern tstADCCConfig stADCC1;
extern tstADCCConfig stADCC2;
extern tstADCCConfig stADCC3;
extern tstADCCConfig stADCC4;



#endif /* ADC_C_H_ */
