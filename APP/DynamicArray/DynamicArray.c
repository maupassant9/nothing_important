/*
 * DynamicArray.c
 * Dynamic Array (thread safe) is an array that can increase
 * the size automatically.
 *
 *  Created on: 15 de fev de 2018
 *      Author: dong.x
 */


#include <stdlib.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/runtime/Error.h>
#include "DynamicArray.h"
#include "../BSP/type.h"

/********************************************
* Internal Function Declaration
********************************************/
static void DynamicArrayInc(dyn_array_t * arr);
static void DynamicArrayDec(dyn_array_t * arr);

/********************************************
* Internal Types and Variables
********************************************/


/********************************************
* External Variables
********************************************/

/*=========================================
 * Functions
 *=========================================*/

/*============================================================
 * Function:  DynamicArrayCreate()
 * Description:  Create and Init the Dynamic array
 * Para:
 * 	   >>
 * Return:
 *     >> dynamic_array_t *: dynamic array pointer
 *
 * Change Record:
 *		>> (15/Fev/2018): Creation of the function;
 *
 *============================================================*/
dyn_array_t * DynamicArrayCreate()
{
#ifdef DYN_ARR_WITH_SEMAPHORE
	Semaphore_Params attrs;
	Error_Block eb;
#endif

	dyn_array_t * ptr = malloc(sizeof(dyn_array_t));

	ptr->array = malloc(sizeof(DYNAMIC_ARRAY_TYPE)
			*DYNAMIC_ARRAY_INIT_SZ);

	ptr->pos = 0;
	ptr->alloc_size = DYNAMIC_ARRAY_INIT_SZ;
	ptr->dec_size = ptr->alloc_size >> 2;
#ifdef DYN_ARR_WITH_SEMAPHORE

	attrs.mode = Semaphore_Mode_BINARY_PRIORITY;
	ptr->sema_dyn_arr = Semaphore_create(1, &attrs, &eb);
#endif
	return ptr;
}

/*============================================================
 * Function:  DynamicArrayGet()
 * Description:  Get the value of array
 * Para:
 *     >> dynamic_array_t *: dynamic array pointer
 *     >> uint16_t: index of the array
 * Return:
 *     >>
 * Change Record:
 *		>> (15/Fev/2018): Creation of the function;
 *
 *============================================================*/
DYNAMIC_ARRAY_TYPE DynamicArrayGet(dyn_array_t * arr, uint16_t idx)
{
	DYNAMIC_ARRAY_TYPE * ptr = (DYNAMIC_ARRAY_TYPE *)arr->array;

	if(idx >= arr->alloc_size){
		return NULL;
	}

	return ptr[idx];
}

/*============================================================
 * Function:  DynamicArrayAdd()
 * Description:  Add a value to array, if size <= idx, increase
 * the array size.
 * Para:
 *     >> dynamic_array_t *: dynamic array pointer
 *     >> DYNAMIC_ARRAY_TYPE: add this value
 * Return:
 *     >>
 * Change Record:
 *		>> (15/Fev/2018): Creation of the function;
 *
 *============================================================*/
void DynamicArrayAdd(dyn_array_t * arr, DYNAMIC_ARRAY_TYPE val)
{
	DYNAMIC_ARRAY_TYPE * ptr = (DYNAMIC_ARRAY_TYPE *)arr->array;
#ifdef DYN_ARR_WITH_SEMAPHORE
	Semaphore_pend(arr->sema_dyn_arr, BIOS_WAIT_FOREVER);
#endif

	//if allocated size is less or equal than idx
	if(arr->pos >= arr->alloc_size)
	{
		DynamicArrayInc(arr);
	}

	ptr = (DYNAMIC_ARRAY_TYPE *)arr->array;
	ptr[arr->pos] = val;

	arr->pos++;
#ifdef DYN_ARR_WITH_SEMAPHORE
	Semaphore_post(arr->sema_dyn_arr);
#endif
}


/*============================================================
 * Function:  DynamicArrayInc()
 * Description:  Increase the size of Dynamic Array.
 * Para:
 *     >> dynamic_array_t *: dynamic array pointer
 * Return:
 *     >>
 * Change Record:
 *		>> (15/Fev/2018): Creation of the function;
 *
 *============================================================*/
static void DynamicArrayInc(dyn_array_t * arr)
{
	uint32_t cnt;
	uint32_t new_sz = arr->alloc_size+DYNAMIC_ARRAY_INIT_SZ;
	DYNAMIC_ARRAY_TYPE * old_ptr = (DYNAMIC_ARRAY_TYPE *)arr->array;
	DYNAMIC_ARRAY_TYPE * new_ptr = malloc(new_sz*sizeof(DYNAMIC_ARRAY_TYPE));

	//copy the array
	for(cnt = 0; cnt < arr->alloc_size; cnt++)
	{
		new_ptr[cnt] = old_ptr[cnt];
	}
	//change the array parameters
	arr->dec_size = new_sz >> 2;
	arr->alloc_size = new_sz;
	//free the old array
	free(old_ptr);
	arr->array = new_ptr;
}

/*============================================================
 * Function:  DynamicArrayDel()
 * Description:  Delete the last value of the array
 * Para:
 *     >> dynamic_array_t *: dynamic array pointer
 *     >> uint32_t: delete from this position until the end of
 *     array.
 * Return:
 *     >>
 * Change Record:
 *		>> (15/Fev/2018): Creation of the function;
 *
 *============================================================*/
void DynamicArrayDel(dyn_array_t * arr)
{
#ifdef DYN_ARR_WITH_SEMAPHORE
	Semaphore_pend(arr->sema_dyn_arr, BIOS_WAIT_FOREVER);
#endif
	arr->pos--;

	if(arr->pos <= arr->dec_size)
	{
		// decrease the size of array
		DynamicArrayDec(arr);
	}
#ifdef DYN_ARR_WITH_SEMAPHORE
	Semaphore_post(arr->sema_dyn_arr);
#endif
}

/*============================================================
 * Function:  DynamicArrayDec(arr)
 * Description:  Decrease the array
 * Para:
 *     >> dynamic_array_t *: dynamic array pointer
 * Return:
 *     >>
 * Change Record:
 *		>> (15/Fev/2018): Creation of the function;
 *
 *============================================================*/
static void DynamicArrayDec(dyn_array_t * arr)
{
	uint32_t cnt;
	uint32_t new_sz = arr->alloc_size-DYNAMIC_ARRAY_INIT_SZ;
	DYNAMIC_ARRAY_TYPE * old_ptr = (DYNAMIC_ARRAY_TYPE *)arr->array;
	DYNAMIC_ARRAY_TYPE * new_ptr;

	if(new_sz <= DYNAMIC_ARRAY_INIT_SZ) return;
	new_ptr = (DYNAMIC_ARRAY_TYPE *)malloc(new_sz*sizeof(DYNAMIC_ARRAY_TYPE));

	//copy the array
	for(cnt = 0; cnt < new_sz; cnt++)
	{
		new_ptr[cnt] = old_ptr[cnt];
	}
	//change the array parameters
	arr->dec_size = new_sz >> 2;
	arr->alloc_size = new_sz;
	//free the old array
	free(old_ptr);
	arr->array = new_ptr;
}


/*============================================================
 * Function:  DynamicArrayDestroy()
 * Description:  Destroy the Dynamic array
 * Para:
 * 	   >> dynamic_array_t *: dynamic array pointer
 * Return:
 *     >>
 *
 * Change Record:
 *		>> (15/Fev/2018): Creation of the function;
 *
 *============================================================*/
void DynamicArrayDestroy(dyn_array_t * arr)
{
#ifdef DYN_ARR_WITH_SEMAPHORE
	Semaphore_delete(&arr->sema_dyn_arr);
#endif
	free(arr->array);
	free(arr);
}


/*============================================================
 * Function:  DynamicArrayReset()
 * Description:  Reset the dynamic array
 * Para:
 * 	   >> dynamic_array_t *: dynamic array pointer
 * Return:
 *     >>
 *
 * Change Record:
 *		>> (15/Fev/2018): Creation of the function;
 *
 *============================================================*/
void DynamicArrayReset(dyn_array_t * arr)
{
#ifdef DYN_ARR_WITH_SEMAPHORE
	Semaphore_pend(arr->sema_dyn_arr, BIOS_WAIT_FOREVER);
#endif
	free(arr->array);
	arr->array = malloc(sizeof(DYNAMIC_ARRAY_TYPE)
				*DYNAMIC_ARRAY_INIT_SZ);

	arr->pos = 0;
	arr->alloc_size = DYNAMIC_ARRAY_INIT_SZ;
	arr->dec_size = arr->alloc_size >> 2;
#ifdef DYN_ARR_WITH_SEMAPHORE
	Semaphore_post(arr->sema_dyn_arr);
#endif
}
