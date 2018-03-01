/*
 * watchdog.h
 *
 *  Created on: 11 de dez de 2017
 *      Author: dong.x
 */

#ifndef BSP_DRIVERS_WATCHDOG_WATCHDOG_H_
#define BSP_DRIVERS_WATCHDOG_WATCHDOG_H_



#define MISSED_TIME_MAX 3
#define ADDR_DSP_WATCHDOG 0x00010101

/*============================================================
 * Function: DrvWatchDogInit()
 * Usage: initiate the GPIO and PSC used by watchdog & data
 * structure of watchdog.
 * Para:
 *    >> watchdog_handle_t: watchdog handle
 * Return:
 *     >>
 * Change Record:
 *		>> (30/Jan/2018): Creation of the function;
 *
 *============================================================*/
void DrvWatchDogInit(watchdog_handle_t * handle);

#ifndef OMAPL138_DSP
/*============================================================
 * Function: DrvWatchDogFeed()
 * Description: Feeds dog, the watchdog Service thread should
 * call this function to feed the watchdog.
 * For ARM, compare the mark and code, if it equal, feeds the
 * dog, and then update the password.
 * For DSP, compare the mark and code, if equal, mark the code.
 * Para:
 *    >> watchdog_handle_t: watchdog handle
 * Return:
 *     >>
 * Change Record:
 *		>> (05/Fev/2018): Creation of the function;
 *
 *============================================================*/
void DrvWatchDogFeed(watchdog_handle_t * handle);
#endif

/*============================================================
 * Function: DrvWatchDogGiveFood()
 * Description: Give some food to feed the dog.
 * For ARM/DSP, write the code into mark. This should be called
 * by all the threads in ARM and DSP.
 * Para:
 *    >> watchdog_handle_t: watchdog handle
 * Return:
 *     >>
 * Change Record:
 *		>> (05/Fev/2018): Creation of the function;
 *
 *============================================================*/
void DrvWatchDogGiveFood(watchdog_handle_t * handle);


#endif /* BSP_DRIVERS_WATCHDOG_WATCHDOG_H_ */
