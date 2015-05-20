#include <math.h>
#include "cmsis_boot/stm32f4xx.h"
#include "measurements.h"

float getOffsetDC(volatile uint16_t adc2Val[], int size, int offset, int numChan);

float convertTemp(int t) {
    float temperature = 0;
    
    temperature = (t * 0.0732421); // ADC conversion to F
    temperature = (temperature - 32) / 1.8; // F to C
    
    return temperature;
}

/*
 * measureOutput()
 * @desc:
 * @param:
 * adc2Val[] = copy of ADC2 conversion array
 * size = DMA buffer size of ADC2
 * offset = Starting index of desired ADC2 channel
 * numChan = Number of channels enabled on ADC2
 */
SENSORS measureOutput(volatile uint16_t adc2Val[], int size, int offset, int numChan) {
    SENSORS out;
    int i = 0;
    int transition1 = 0;
    int transition2 = 0;
    //float dcOffset = getOffsetDC(adc2Val,size,offset,numChan);
    float voltage = 0;
    float vSqrd = 0;
    float vrms = 0;
    float period = 0;
    //float preVoltage = (adc2Val[i+offset] * 0.000732421);
    char triggered = 0;
    
    for(i = 0 + offset; i < size; i += numChan) {
        voltage = adc2Val[i] * 0.000732421;
        
        vSqrd = vSqrd + (voltage * voltage);
        
        // check for transition and rising edge
        if(voltage > 2) {
            if(!triggered){
                triggered = 1;
                if(!transition1) {
                    transition1 = i; // index of first transition
                }
                else if(transition1 && !transition2) {
                    transition2 = i; // index of second transition
                }
            }
        } else if(voltage < 1) {
            triggered = 0;
        }
        //preVoltage = newVoltage; // track previous voltage
    }
    
    vrms = sqrt(vSqrd/(size/numChan));
    out.outVoltage = vrms;
    
    // calculate the period based on signal sample rate
    if(!transition1 || !transition2) {
        out.outFreq = 0;
    } else {
        period = ((transition2 - transition1)/numChan) * 0.0000205;
        out.outFreq = 1/period;
    }    
       
    return out;
}

float getOffsetDC(volatile uint16_t adc2Val[], int size, int offset, int numChan) {
    int i = 0;
    //int index = 0 + offset;
    float newVoltage = 0;
    float lowest = adc2Val[i+offset] * 0.000732421;
    float highest = 0;
    
    for (i = 0 + offset; i < size; i += numChan){
        newVoltage = adc2Val[i] * 0.000732421;
        if(newVoltage < lowest){
            lowest = newVoltage;
        }
        
        if(newVoltage > highest) {
            highest = newVoltage;
        }
        
        //index += numChan;
    }
    
    return (highest - lowest)/2;
}
