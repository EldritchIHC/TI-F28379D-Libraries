/*
 * PWM.h
 *
 *  Created on: 12 feb. 2023
 *      Author: Paul
 */

#ifndef PWM_H_
#define PWM_H_

/*Defines*/
#define UP 1
#define DOWN 2
#define UPDOWN 3

#define SOCZERO 1
#define SOCPRD 2
#define SOCZPRD 3
#define SOCCMPAU 4
#define SOCCMPAD 5
#define SOCCMPBU 6
#define SOCCMPBD 7

/*Structures*/


typedef struct{
    Uint32 u32Freq_hz;
    Uint32 u32Duty_percent;//0-100
    Uint16 u16DeadBandRED_ns;
    Uint16 u16DeadBandFED_ns;
    Uint32 u32TBPRD;
}tstEPWMConfig;



/*Structure Initializations*/
//PWM4
#define PWM1_INIT {HC_SWITCHING_FREQUENCY, DUTYMIN, 20, 25}
//PWM5
#define PWM2_INIT {HC_SWITCHING_FREQUENCY, DUTYMIN, 20, 25}
//PWM6
#define PWM3_INIT {BC_SWITCHING_FREQUENCY, DUTYMIN, 25, 25}
/*Function prototypes*/
void PWM_vEPWMModuleInit();

void PWM_vEPWM4Init();
void PWM_vEPWM4Config(tstEPWMConfig *PWM);
void PWM_vEPWM4ConverterMode();
void PWM_vEPWM4DutyUpdate(tstEPWMConfig *PWM, Uint16 u16newDuty);
void PWM_vEPWM4DeadbandUpdate(tstEPWMConfig *PWM);
void PWM_vEPWM4Start();
void PWM_vEPWM4Stop();
void PWM_EPWM4GlobalLoad();
void PWM_vEPWM4TripZoneStop();
void PWM_vEPWM4TripZoneClear();
void PWM_vEPWM4SOCEnable();

void PWM_vEPWM5Init();
void PWM_vEPWM5Config(tstEPWMConfig *PWM);
void PWM_vEPWM5ConverterMode();
void PWM_vEPWM5DutyUpdate(tstEPWMConfig *PWM, Uint16 u16newDuty);
void PWM_vEPWM5DeadbandUpdate(tstEPWMConfig *PWM);
void PWM_vEPWM5Start();
void PWM_vEPWM5Stop();
void PWM_vEPWM5TripZoneStop();
void PWM_vEPWM5TripZoneClear();
void PWM_vEPWM5SOCEnable();

void PWM_vEPWM_Global_Start();
void PWM_vEPWM_Global_Stop();

interrupt void epwm4_isr(void);
interrupt void epwm5_isr(void);
/*Variables*/
extern tstEPWMConfig stPWM1;
extern tstEPWMConfig stPWM2;
extern tstEPWMConfig stPWM3;

#endif /* PWM_H_ */
