/*
 * DynamicArray.h
 *
 *  Created on: 15 de fev de 2018
 *      Author: dong.x
 */

#ifndef APP_DYNAMICARRAY_DYNAMICARRAY_H_
#define APP_DYNAMICARRAY_DYNAMICARRAY_H_

#include "../BSP/type.h"
#include <ti/sysbios/knl/Semaphore.h>

//Comment the following line if semaphore
//should not be used.
//#define  DYN_ARR_WITH_SEMAPHORE

//The type of data saved in dynamic array:
// ONLY these types accepted:
// * char
// * int
// * short
// * long
// * float/double
#define DYNAMIC_ARRAY_TYPE uint8_t
//Initial array size in unit of type of data.
#define DYNAMIC_ARRAY_INIT_SZ 30


typedef struct dyn_arr_str_t{
#ifdef DYN_ARR_WITH_SEMAPHORE
	Semaphore_Handle sema_dyn_arr;
#endif
	//pointer to current array position
	int32_t pos;
	//current allocated size of the array
	int32_t alloc_size;
	//threshold for decrease the array
	int32_t dec_size;
	//pointer to memory area
	void * array;
} dyn_array_t;



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
dyn_array_t * DynamicArrayCreate();


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
DYNAMIC_ARRAY_TYPE DynamicArrayGet(dyn_array_t * arr, uint16_t idx);


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
void DynamicArrayAdd(dyn_array_t * arr, DYNAMIC_ARRAY_TYPE val);


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
void DynamicArrayDel(dyn_array_t * arr);


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
void DynamicArrayDestroy(dyn_array_t * arr);

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
void DynamicArrayReset(dyn_array_t * arr);

#endif /* APP_DYNAMICARRAY_DYNAMICARRAY_H_ */
