/*
 * AdmEvent.c
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
#include "admevent.h"

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
//Queue and mailbox for communication between
//AdmObservador and AdmEvent. Semaphore for
//queue_err, used to sync the AdmErrors.
extern Queue_Handle queue_err;
extern Semaphore_Handle sema_err;
extern Mailbox_Handle mailbox_event;

/*=========================================
 * Threads
 *=========================================*/

/*============================================================
 * Thread Name: AdmEventos
 * Type: Static Thread
 * Priority: 24
 * Description: Administrator of events.
 * 1. Receive the events.
 * 2. Handling the events.
 * 3. Free all the received events.
 * Change Record:
 *		>> (15/Fev/2018): AdmEvent thread created.
 *============================================================*/
void AdmEventos(void)
{
	eb_t * evt;

	while(true){
		//Get from Mailbox the event
		Mailbox_pend(mailbox_event, evt, BIOS_WAIT_FOREVER);
		board->watchdog->GiveFood(board->watchdog);
#ifdef DEBUG
		board->uart0->WriteString(board->uart0, "Handling Event.");
#endif

		//handling the events
		switch(evt->id)
		{
			case EVT_TASK_INIT_END:
				//Delete the TaskInit

				break;
			case 2: break;
		}


		//free the event
		free(evt);
	}
}


/*============================================================
 * Thread Name: AdmErros
 * Type: Static Thread
 * Priority: 24
 * Description: Administrator of errors.
 * 1. Receive all of the errors.
 * 2. Handling all the errors.
 * 3. Free all the received errors.
 * Change Record:
 *		>> (15/Fev/2018): AdmEvent thread created.
 *============================================================*/
void AdmErros(void)
{
	eb_t * err = 0x0102;

	while(true){
		//Get Semaphore
		Semaphore_pend(sema_err, BIOS_WAIT_FOREVER);
		board->watchdog->GiveFood(board->watchdog);
		while((err = Queue_dequeue(queue_err)) != NULL){

#ifdef DEBUG
			board->uart0->WriteString(board->uart0,
					"Handling Errors.");
#endif

			//handling the errors
			switch(err->id)
			{
				case 0: break;
				case 1: break;
				case 2: break;
			}

			//free the error
			free(err);
		}
	}
}
