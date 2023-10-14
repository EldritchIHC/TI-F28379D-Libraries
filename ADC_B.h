/*
 * ADC_B.h
 *
 *  Created on: 18 feb. 2023
 *      Author: ihate
 */

#ifndef ADC_B_H_
#define ADC_B_H_


/*Defines*/

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
/*Structures*/
typedef struct{
    Uint16 u16ADCChannel;//Input Pin
    Uint16 u16AqWindow;//
    Uint16 u16Interrupt;//Interrupt number 1 - 4
    Uint16 u16TriggerSRC;//Trigger Source(EPWM/CPU)
}tstADCBConfig;
/*Structure Initializations*/
/*
    Hardware - Software Interface
    *48V Voltage Measurement - ADCINB4
    *12V Voltage Measurement  - ADCINB3
*/

//12V Voltage Measurement
#define ADCB1_INIT_BUCK {3, 14, 1, EPWM4SOCA}
//48V Voltage Measurement
#define ADCB1_INIT_BOOST {4, 14, 1, EPWM4SOCA}
/*Function prototypes*/
void ADC_B_vADCB_Group_Init();
void ADC_B_vADCB_SOC0Config(tstADCBConfig ADC);
void ADC_B_vADCB_SOC1Config(tstADCBConfig ADC);
Uint16 ADC_B_u16ADCBgetRawValue0();
Uint16 ADC_B_u16ADCBgetRawValue1();
/*Variables*/
extern tstADCBConfig stADCB1;
extern tstADCBConfig stADCB2;
extern tstADCBConfig stADCB3;
extern tstADCBConfig stADCB4;


#endif /* ADC_B_H_ */
