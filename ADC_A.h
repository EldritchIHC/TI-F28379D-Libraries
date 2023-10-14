/*
 * ADC_A.h
 *
 *  Created on: 12 feb. 2023
 *      Author: ihate
 */

#ifndef ADC_A_H_
#define ADC_A_H_


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
}tstADCAConfig;

/*Structure Initializations*/
/*
    Hardware - Software Interface
    *48V Current Measurement  - ADCINA4
    *12V Current Measurement - ADCINA3
*/
//12V Current Measurement
#define ADCA1_INIT_BUCK  {3, 14, 1, EPWM4SOCA}
//48V Current Measurement
#define ADCA1_INIT_BOOST  {4, 14, 1, EPWM4SOCA}
/*Function prototypes*/
void ADC_A_vADCA_Group_Init();
void ADC_A_vADCA_SOC0Config(tstADCAConfig ADC);
void ADC_A_vADCA_SOC1Config(tstADCAConfig ADC);
Uint16 ADC_A_u16ADCAgetRawValue0();
Uint16 ADC_A_u16ADCAgetRawValue1();
/*Variables*/
extern tstADCAConfig stADCA1;
extern tstADCAConfig stADCA2;
extern tstADCAConfig stADCA3;
extern tstADCAConfig stADCA4;


#endif /* ADC_A_H_ */
