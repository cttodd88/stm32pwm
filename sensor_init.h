/* 
 * File:   sensor_init.h
 * Author: elex
 *
 * Created on May 6, 2015, 12:33 PM
 */

#ifndef SENSOR_INIT_H
#define	SENSOR_INIT_H

#ifdef	__cplusplus
extern "C" {
#endif

    void GPIO_init(void);
    void ADC_init(void);
    void IT_init();
    void DMA_init(__IO uint16_t ADC3Value);
    


#ifdef	__cplusplus
}
#endif

#endif	/* SENSOR_INIT_H */

