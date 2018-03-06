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
uint16_t res[1024];
uint16_t res2[1024];
uint32_t val[5], cnt2 = 0;
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

void IsrAdc()
{
//	board->adc1->Start(board->adc1);
//	board->adc1->GetResult(board->adc1, &val);
//	res[cnt2] = (val>>16);
//	res2[cnt2++] = val&0xffff;
//	if(cnt2 == 1024)
//	{
//		cnt2 = 0;
//	}

	board->tdc->Start(board->tdc);
	//chk tdc int here

	board->tdc->GetResult(board->tdc, &val);
	//point to next channel
	//board->lacos->Next(board->lacos);
	board->tdc->Next(board->tdc);

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
	//board->lacos->Start(board->lacos);

    BIOS_start();    /* does not return */
    return(0);
}
