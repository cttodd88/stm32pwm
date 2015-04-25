/*
 * delays.c
 *
 *  Created on: Apr 26, 2012
 *      Author: Dad
 */


static unsigned int TimingDelay;

/*
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void DelayMs(unsigned int nTime)
{
	  TimingDelay = nTime;

	  while(TimingDelay != 0);
}


/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

