/*
 * delays.h
 *
 *  Created on: Apr 26, 2012
 *      Author: Mel Dundas
 */

#ifndef DELAYS_H_
#define DELAYS_H_

static volatile uint32_t TimingDelay;

void DelayMs(unsigned int nTime);
void TimingDelay_Decrement(void);


#endif /* DELAYS_H_ */
