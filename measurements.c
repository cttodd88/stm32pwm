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
SENSORS measureOutput(volatile uint16_t adc2Val[], int size) {
    SENSORS out;
    int i = 0;
    int transition1 = 0;
    int transition2 = 0;
    float voltage = 0;
    float vSqrd = 0;
    float vrms = 0;
    float period = 0;
    char triggered1 = 1;
    char triggered2 = 1;
    char chanSelect = 0;
    char outTransition = 0;
    
    for(i = 0; i < size; i++) {
        voltage = adc2Val[i] * 0.000732421;
        
        if(!chanSelect) {
            vSqrd = vSqrd + (voltage * voltage);
        }
                
        // check for transition and rising edge
        if(voltage > 2) {
            if(!triggered1 && !chanSelect){
                triggered1 = 1;
                if(!transition1) {
                    transition1 = i; // index of first transition
                } else if(transition1 && !transition2) {
                    transition2 = i; // index of second transition
                }
            } else if(!triggered2 && !outTransition && chanSelect) {
                triggered2 = 1;
                outTransition = i;
            }
        } else if(voltage < 1) {
            if(!chanSelect) {
                triggered1 = 0;
            } else {
                triggered2 = 0;
            }           
        }
        //preVoltage = newVoltage; // track previous voltage
        
        //toggle channel
        chanSelect ^= 0x01;
    }
    
    vrms = sqrt(vSqrd/(size/2));
    out.outVoltage = vrms;
    
    // calculate the period based on signal sample rate
    if(!transition1 || !transition2) {
        out.outFreq = 0;
    } else {
        period = ((transition2 - transition1)/2) * 26.5; //0.0000205;
        out.outFreq = 1000000/period;
    }    
    
    if((transition1 < (outTransition - 1)) && outTransition) {
        out.outLagLead = 0x02; // leading
        out.delay = outTransition - transition1;
    } else if ((transition1 > (outTransition - 1)) && outTransition) {
        out.outLagLead = 0x01; // lagging
        out.delay = (transition1 - outTransition) * 26.5; // us
    } else {
        out.outLagLead = 0; // in phase
        out.delay = 0;
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
