/*
 * AdmObservador.c
 *
 *  Created on: 9 de fev de 2018
 *      Author: dong.x
 */

/********************************************
* Include
********************************************/
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/mailbox.h>
#include <stdlib.h>
#include "../BSP/board.h"
#include "../BSP/Drivers/Error/err.h"
#include "AdmObservador.h"
#include "../App/DynamicArray/dynamicarray.h"
#include "../APP/AdmComm/admcomm.h"

/********************************************
* Internal Function Declaration
********************************************/

/********************************************
* Internal Types and Variables
********************************************/
//A counter for internal use.
static unsigned char cnt = 0;
/********************************************
* External Variables
********************************************/
extern board_handle_t * board;
//Queue and mailbox for communication between
//AdmObservador and AdmEvent.
extern Queue_Handle queue_err;
extern Mailbox_Handle mailbox_event;
//Queue to transfer data from this thread to
//AdmComm.
extern Mailbox_Handle mailbox_comm;
//A dynamic array for COMM thread
//extern dyn_array_t * dyn_arr_comm;



/*=========================================
 * Thread
 *=========================================*/

/*============================================================
 * Thread Name: AdmObservador
 * Type: Static Swi
 * Priority: 6
 * Description: Observer.
 * 1. Monitor the hardware configuration changes.
 * 2. Updates the leds.
 * 3. Feed watchdog.
 * 4. Get all the uart data if possible and package it
 * accordingly.
 * Change Record:
 *		>> (15/Fev/2018): AdmObservador thread created.
 *============================================================*/
void AdmObservador()
{
	eb_t *err; //error structure
	dyn_array_t * arr; //a dynamic array

	cnt++;

	//1 - Uart handling:Check if has char arrived
	//TODO: Needs to limit the command sz to be received??
	while(board->uart0->HasChar(board->uart0)){
		arr = DynamicArrayCreate();
		DynamicArrayAdd(arr,
			(uint8_t)board->uart0->ReadChar(board->uart0));
	}
	//Snd to Mailbox_cmd, if failed, clean the memory.
	if(!Mailbox_post(mailbox_comm,arr,0))
		DynamicArrayDestroy(arr);

	//2 - Watchdog handling: feed dog
	if(cnt == WATCHDOG_THRESHOLD_VAL)
	{
		//watchdog feed here.
		board->watchdog->Feed(board->watchdog);
	}

	//3 - LEDs handling: update LEDs state
	if(cnt == LEDS_THRESHOLD_VAL)
	{
		// led here
		board->leds->Update(board->leds);
	}

	//4 - DIPSWITCH handling:Send a event to
	//    AdmEvento thread.
	if(cnt == DIPSWITCH_THRESHOLD_VAL)
	{
		//dipswitch here.
		board->dipswitch->Read(board->dipswitch);
		if(board->dipswitch->has_changed)
		{
			//Generate Event handle
			err = (eb_t *)malloc(sizeof(eb_t));
			err->type = ERR_TYPE_EVT;
			err->data = NULL;
			err->id = DIP_SWITCH_CHANGED;
			//Send to AdmEvent
			if(!Mailbox_post(mailbox_event,err,0))
				free(err);
		}
	}

	if(cnt == CNTER_MAX_VAL) cnt = 0;
}
