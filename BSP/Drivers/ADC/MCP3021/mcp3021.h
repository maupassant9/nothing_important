/* 
* FileName: mcp3021.h
* Author: Dong Xia 
* This is head file of mcp3021.
* 
* Change Records: 
*      >> (06/Fev/2018): Creation of file 
* 
*/


#ifndef  BSP_DRIVERS_MCP3021_MCP3021_H_
#define  BSP_DRIVERS_MCP3021_MCP3021_H_
/********************************************
* Include 
********************************************/
#include "../BSP/connector.h"

/********************************************
* Macro 
********************************************/
#define MCP3021_ADDR 0x4d  //I2C addr of dev


/********************************************
* Type definition 
********************************************/


/********************************************
* Function prototype 
********************************************/

/*------------------------------------------------ 
* Funcname: DrvMcp3021Init(); 
* Usage: Initiate the dev and data structure
* Paras: 
*  >> adc_handle_t :  an adc handle.
*  >> adc_conf_t :  an adc configuration structure.
* Return: 
*  >> : 
* Change Records: 
*  >> (07/Jan/2017): Create the function 
*----------------------------------------------*/ 
 void DrvMcp3021Init(adc_handle_t * handle, void * conf);

 /*------------------------------------------------ 
* Funcname: DrvMcp3021Start(); 
* Usage: Start conversion.
* Paras: 
*  >> adc_handle_t :  an adc handle.
* Return: 
*  >> : 
* Change Records: 
*  >> (07/Jan/2017): Create the function 
*----------------------------------------------*/ 
void DrvMcp3021Start(adc_handle_t * handle);

/*============================================================
 * Function:  DrvMcp3021GetResult
 * Description: Get the conversion result.
 * Para:
 *    >> adc_handle_t :  an adc handle.
 * Return:
 *     >>
 * Change Record:
 *		>> (07/Jan/2018): Creation of the function;
 *
 *============================================================*/
void DrvMcp3021GetResult(adc_handle_t * handle, uint32_t * val);

/*============================================================
 * Function:  DrvMcp3021NotSupported
 * Description: Enter automatic mode, using DMA
 * !Not supported, and no effect! Not plan to realize this 
 * function in future.
 * Para:
 *    >> adc_handle_t :  an adc handle.
 * Return:
 *     >>
 * Change Record:
 *		>> (07/Jan/2018): Creation of the function;
 *
 *============================================================*/
void DrvMcp3021NotSupported(adc_handle_t * handle);


/*============================================================
 * Function:  DrvMcp3021Link
 * Description: Link the MCP3021 to I2C
 * Para:
 *    >> adc_handle_t :  an adc handle.
 *    >> void *: an i2c handle
 * Return:
 *     >>
 * Change Record:
 *		>> (08/Jan/2018): Creation of the function;
 *
 *============================================================*/
void DrvMcp3021Link(adc_handle_t * handle, void * comm);

#endif /* BSP_DRIVERS_MCP3021_MCP3021_H_ */
