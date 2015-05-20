/* 
 * File:   measurements.h
 * Author: elex
 *
 * Created on May 15, 2015, 10:48 AM
 */

#ifndef MEASUREMENTS_H
#define	MEASUREMENTS_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct{
    // inputs
    float inVoltage;
    float inCurrent;
    float inPower;
    // outputs
    float outVoltage;
    float outCurrent;
    float outPower;
    float outFreq;
    // temperatures
    float temp1;
    float temp2;
} SENSORS;

float convertTemp(int t);
SENSORS measureOutput(volatile uint16_t adc2Val[], int size, int offset, int numChan);

#ifdef	__cplusplus
}
#endif

#endif	/* MEASUREMENTS_H */

