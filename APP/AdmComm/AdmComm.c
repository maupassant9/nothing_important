/*
 * AdmComm.c
 *
 *  Created on: 15 de fev de 2018
 *      Author: dong.x
 */


/********************************************
* Include
********************************************/
#include <stdlib.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/mailbox.h>
#include "../BSP/Drivers/Error/err.h"
#include "../BSP/board.h"
#include "../BSP/Drivers/Error/err.h"
#include "../App/DynamicArray/dynamicarray.h"

/********************************************
* Internal Function Declaration
********************************************/

/********************************************
* Internal Types and Variables
********************************************/

/********************************************
* External Variables
********************************************/
extern board_handle_t * board;
extern Mailbox_Handle mailbox_cmd;
/*=========================================
 * Thread
 *=========================================*/

/*============================================================
 * Thread Name: AdmComm
 * Type: Static Thread
 * Priority: 15
 * Description: Administrator of Communication. Communicator.
 * 1. Communication between mother board and this board.
 * 2. Handling the command received.
 * Change Record:
 *		>> (15/Fev/2018): AdmEvent thread created.
 *============================================================*/
void AdmComm(void)
{
	dyn_array_t * uart_data;

	while(true){
		//wait in mailbox
		Mailbox_pend(mailbox_cmd, uart_data, BIOS_WAIT_FOREVER);
#ifdef DEBUG
		board->uart0->WriteString(board->uart0, "Enter AdmComm.");
#endif
		board->watchdog->GiveFood(board->watchdog);
		//Check uart data - command or data



		//destroy the dynamic array
		DynamicArrayDestroy(uart_data);
	}
}
