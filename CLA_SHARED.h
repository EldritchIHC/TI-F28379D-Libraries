/*
 * CLA_HEADER.h
 *
 *  Created on: 22 feb. 2023
 *      Author: Paul
 */

#ifndef CLA_SHARED_H_
#define CLA_SHARED_H_



/*Defines*/

/*Structures*/
typedef struct{
    float fError[5];
    float fDuty[5];
    float fACoeffs[5];
    float fBCoeffs[5];
}tstCLAData;
/*Function prototypes*/
__interrupt void Cla1Task1();
__interrupt void Cla1Task2();
__interrupt void Cla1Task3();
__interrupt void Cla1Task4();
__interrupt void Cla1Task5();
__interrupt void Cla1Task6();
__interrupt void Cla1Task7();
__interrupt void Cla1Task8();
/*Variables*/
//To read from CLA
extern tstCLAData stCLAData1;
//To read from CPU
extern float fVoltageMeasurement;

#endif /* CLA_HEADER_H_ */
