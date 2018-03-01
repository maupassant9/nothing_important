/*
 *  ======== main.c ========
 */

/* ================ SYS/BIOS include ==============*/
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

/* ================ XDC include ===================*/
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/log.h>
#include <xdc/runtime/Error.h>

/* ================ BSP include ===================*/
#include "../BSP/board.h"
#include "../BSP/connector.h"
//#include "../BSP/Drivers/UART/uart.h"
#include "include/gpio.h"
#include "include/hw/soc_OMAPL138.h"
#include "include/armv5/omapl138/interrupt.h"
#include "include/edma.h"
#include "../APP/DynamicArray/DynamicArray.h"
#include "../BSP/Drivers/ADC/AD7265/ad7265.h"
#include "../BSP/Drivers/DIPSWITCH/dipswitch.h"


#define FREQ_TST
/* ================ External Variables=============*/
extern Semaphore_Handle sem;
//static unsigned char value = 0;
board_handle_t * board;
dyn_array_t * dyn_arr_comm;
uint8_t cnt = 0;

/*
 *  ======== taskFxn ========
 */
void IsrClock()
{
	if((cnt++)%2 == 0)
	{
		board->leds->Set(board->leds, 0x7);
	}
	else {
		board->leds->Clr(board->leds, 0x7);
	}

	board->leds->Update(board->leds);
}



/*
 *  ======== main ========
 */
Int main()
{ 
	int laco_no = 1;
	unsigned int res = 0;
	char cap_val = 0x01;
    /*
     * use ROV->SysMin to view the characters in the circular buffer
     */
    board = BoardInit();

//    while(1){
//    	if(cap_val == 0) cap_val = 1;
//    	board->lacos->SetCaps(board->lacos,laco_no,0x01);
//    	cap_val = cap_val << 1;
//    }
    //dyn_arr_comm = DynamicArrayCreate();
    BIOS_start();    /* does not return */
    return(0);
}
