/*
 * DAC.h
 *
 *  Created on: 6 mar. 2023
 *      Author: Paul
 */

#ifndef DAC_H_
#define DAC_H_


/*Defines*/
#define REFERENCE_VDAC  0
#define REFERENCE_VREF  1

#define EPWM4SYNCPER 4
#define EPWM5SYNCPER 5

#define SYSCLK 0
#define SYNCSELCLK 1

#define MAX12BIT 4095U
//DACA at ADCINA0, DACB at ADCINA1
enum DACCHANNEL{ CHANNEL_A, CHANNEL_B, CHANNEL_C};
/*Structures*/
typedef struct{
    Uint16 u16Value;
    float fSlope;
    int16 i16Offset;
    Uint16 u16DACChannel;
}tstDACData;
/*Function prototypes*/
void DAC_vDACInit();
void DAC_vDACWriteValue(tstDACData DACData);
/*Variables*/
extern tstDACData stDACData1;
extern tstDACData stDACData2;
#endif /* DAC_H_ */
