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
    // for synch purposes
    char outLagLead; // 0 = in phase, 1 = lagging, 2 = leading
    int delay; // delay in index trigger
} SENSORS;

float convertTemp(int t);
SENSORS measureOutput(volatile uint16_t adc2Val[], int size);

#ifdef	__cplusplus
}
#endif

#endif	/* MEASUREMENTS_H */

