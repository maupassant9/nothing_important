/*
 * TasksInit.c
 * Task for Initialization.
 *
 *  Created on: 16 de fev de 2018
 *      Author: dong.x
 */



/********************************************
* Include
********************************************/
#include <stdlib.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/mailbox.h>
#include "../BSP/Drivers/Error/err.h"
#include "../BSP/board.h"

/********************************************
* Internal Function Declaration
********************************************/
static void SelfExam(void);
static void BootDsp(void);
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
extern Mailbox_Handle mailbox_event;

/*=========================================
 * Threads
 *=========================================*/

/*============================================================
 * Thread Name: TaskInit
 * Type: Dynamic Thread
 * Priority: 10
 * Description: Initiate the board and self test.
 * Change Record:
 *		>> (16/Fev/2018): TaskInit created
 *============================================================*/
void TaskInit(void)
{
	eb_t *err, *evt;

	//1 - Init board
	BoardInit();

	board->watchdog->GiveFood(board->watchdog);

#ifdef DEBUG
	board->uart0->WriteString(board->uart0, "Task Inited...");
#endif

	//2 - Self-exam
	SelfExam();

	//3 - Boot Dsp
	BootDsp();

	//inform AdmEvent
	evt = (eb_t *)malloc(sizeof(eb_t));
	evt->type = ERR_TYPE_EVT;
	Mailbox_post(mailbox_event, evt, BIOS_WAIT_FOREVER);

	//delete the task.
	Task_exit();
}


static void SelfExam()
{
#ifdef DEBUG
	board->uart0->WriteString(board->uart0, "TODO: Self Exam...");
#endif
	//Self Test here ...

}


static void BootDsp()
{
#ifdef DEBUG
	board->uart0->WriteString(board->uart0, "TODO: Boot DSP...");
#endif
	//Boot the DSP ...

}

