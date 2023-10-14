/*
 * SYSCONFIG.h
 *
 *  Created on: 18 feb. 2023
 *      Author: ihate
 */

#ifndef SYSFUNCTIONS_H_
#define SYSFUNCTIONS_H_


/*Defines*/
//#define TEMPMEAS1 0x01
//#define TEMPMEAS2 0x02
//#define TEMPMEAS3 0x04
/*Structures*/
typedef struct{
    int16 i16TempMeas1;
    int16 i16TempMeas2;
    int16 i16TempMeas3;
}tstTempMeasurements;

/*Function prototypes*/
void SYSFUNCTIONS_vInitialConfig();
void SYSFUNCTIONS_vFinalConfig();
void SYSFUNCTIONS_vTemp1Calculate(tstTempMeasurements *TempMeasurements);
void SYSFUNCTIONS_vTemp2Calculate(tstTempMeasurements *TempMeasurements);
void SYSFUNCTIONS_vTemp3Calculate(tstTempMeasurements *TempMeasurements);
void SYSFUNCTIONS_vConverterStatus();
void SYSFUNCTIONS_vConverterUpdate();
/*Variables*/
extern tstTempMeasurements stTempMeasurements;

#endif /* SYSFUNCTIONS_H_ */
