/*
* FileName: watchdog.c
* Author: Dong Xia
* This is source file of watchdog.
*
* Change Records:
*      >> (30/Jan/2018): file created.
*
*/

/********************************************
* Include
********************************************/
#include "../BSP/type.h"
#include "../BSP/conf.h"
#include "../BSP/connector.h"
#include "../BSP/board.h"
#include <stdlib.h>

#include "hw_types.h"
#include "include/hw/soc_OMAPL138.h"
#include "hw_syscfg0_OMAPL138.h"
#include "include/armv5/omapl138/interrupt.h"
#include "include/psc.h"
#include "include/gpio.h"
#include "watchdog.h"

/********************************************
* Internal Function Declaration
********************************************/


/********************************************
* Internal Types and Variables
********************************************/


/********************************************
* External Variables
********************************************/
//This is a password used to generate code for
// both DSP and ARM, the algorithm is simple
// add one for every feed of watchdog.
static uint8_t password = 0;



/********************************************
* Functions
********************************************/
/*============================================================
 * Function: DrvWatchDogInit()
 * Description: initiate the GPIO and PSC used by watchdog & data
 * structure of watchdog.
 * Para:
 *    >> watchdog_handle_t: watchdog handle
 * Return:
 *     >>
 * Change Record:
 *		>> (30/Jan/2018): Creation of the function;
 *
 *============================================================*/
void DrvWatchDogInit(watchdog_handle_t * handle)
{
    uint32_t save_pin_mutex;

    #ifndef OMAPL138_DSP  /* ARM */
    handle->ptr_wdog_dsp = (watchdog_handle_t *)ADDR_DSP_WATCHDOG;

    #else /* DSP */
    handle->ptr_wdog_dsp = NULL;
    #endif

    // Init the PSC and GPIO
    save_pin_mutex = (HWREG(SOC_SYSCFG_0_REGS +
                    SYSCFG0_PINMUX(WATCHDOG_FEED_PIN_MUX_NUM)) &
	 	            ~(WATCHDOG_FEED_PIN_MUX_LOC));
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(WATCHDOG_FEED_PIN_MUX_NUM)) =
	     			 (WATCHDOG_FEED_PIN_MUX_VAL|save_pin_mutex);

    GPIODirModeSet(SOC_GPIO_0_REGS,WATCHDOG_FEED_PIN,GPIO_DIR_OUTPUT);
    GPIOPinWrite(SOC_GPIO_0_REGS,WATCHDOG_FEED_PIN,GPIO_PIN_HIGH);

    handle->mark = 0;
    handle->code = 0;
    handle->cnter = 0;

}

#ifndef OMAPL138_DSP
/*============================================================
 * Function: DrvWatchDogUpdatePassword()
 * Description: Generate Password for DSP and ARM, the password
 * updates every time the watchdog thread feeds the dog.
 * Para:
 *    >> watchdog_handle_t: watchdog handle
 * Return:
 *     >>
 * Change Record:
 *		>> (05/Fev/2018): Creation of the function;
 *
 *============================================================*/
void WatchDogUpdatePassword(watchdog_handle_t * handle)
{
    password++;
    handle->code = password;
    handle->ptr_wdog_dsp->code = password;
}


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
void DrvWatchDogFeed(watchdog_handle_t * handle)
{
    if(handle->code == handle->mark)
    {
        if(handle->ptr_wdog_dsp->mark == handle->ptr_wdog_dsp->code){
            GPIOPinWrite(SOC_GPIO_0_REGS,WATCHDOG_FEED_PIN,handle->code%2);
            WatchDogUpdatePassword(handle);
        }
    }
}
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
void DrvWatchDogGiveFood(watchdog_handle_t * handle)
{
    handle->mark = handle->code;
}
