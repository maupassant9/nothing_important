/*
 * tdc.h
 *
 *  Created on: 11 de dez de 2017
 *      Author: dong.x
 */

#ifndef BSP_DRIVERS_TDC_TDC_H_
#define BSP_DRIVERS_TDC_TDC_H_

#include "../BSP/type.h"
#include "../BSP/connector.h"

/*============================================================
 * Macro
 *============================================================*/
//===============
// TDC7201 CMDS
//===============
#define AUTO_INCREMENT 0x8000
#define IS_WRITE 0x4000
#define WRITE_CMD 0x4000
//================
// Register address
//================
//8bits register
#define TDC_CFG1 0
#define TDC_CFG2 0x100
#define TDC_INT_STATUS 0x200
#define TDC_INT_MASK 0x300
#define TDC_COARSE_CNTR_OVF_H 0x400
#define TDC_COARSE_CNTR_OVF_L 0x500
#define TDC_CLOCK_CNTR_OVF_H 0x600
#define TDC_CLOCK_CNTR_OVF_L 0x700
#define TDC_CLOCK_CNTR_STOP_MASK_H 0x800
#define TDC_CLOCK_CNTR_STOP_MASK_L 0x900
//24bits register
#define TDC_TIME1 0x1000
#define TDC_CLOCK_COUNT1 0x1100
#define TDC_TIME2 0x1200
#define TDC_CLOCK_COUNT2 0x1300
#define TDC_TIME3 0x1400
#define TDC_CLOCK_COUNT3 0x1500
#define TDC_TIME4 0x1600
#define TDC_CLOCK_COUNT4 0x1700
#define TDC_TIME5 0x1800
#define TDC_CLOCK_COUNT5 0x1900
#define TDC_TIME6 0x1A00
#define TDC_CALIBRATION1 0x1B00
#define TDC_CALIBRATION2 0x1C00

// in ps
#define TDC_CLK_PERIOD 1000000/8
//================
// CHANNEL NUMBER
//================
//CHannel no for INIT 0~3
//The channel no for 4 INT pins
//These 4 int pins should be corresponding
//to the same DMA channel.
#define CHANNEL_NO_INIT_0 6

#define CHANNEL_NO_INIT_1 3
#define CHANNEL_NO_INIT_2 4
#define CHANNEL_NO_INIT_3 2
//Channel no for WR
#define CHANNEL_NO_WR 15
//Channel no for WR Auxiliar 1~3.
#define CHANNEL_NO_WRAUX_1 5
#define CHANNEL_NO_WRAUX_2 0
#define CHANNEL_NO_WRAUX_3 1
//Channel no for READ
#define CHANNEL_NO_RD 14


/*============================================================
 *Typedef
 *============================================================*/

/*============================================================
 *Function Prototype
 *============================================================*/

/*============================================================
 * Function: DrvSpiInit()
 * Para:
 *    >> tdc_handle_t * handle: a tdc handle
 *    >> void * ptr: tdc configuration structure
 * Return:
 *     >>
 *  Usage: Initilization of spi module. The first function
 * should be called before any other spi function.
 *
 *============================================================*/
void DrvTdcInit(tdc_handle_t * handle, void * ptr_conf);


/*============================================================
 * Function: DrvTdcGetResult()
 * Description: Get the result of TDC
 * Para:
 *    >> tdc_handle_t * handle: a tdc7201 handle
 *    >> uint32_t: a pointer of buffer to save the value of results:
 *    	* TDC_TIME1
 *    	* TDC_TIME2
 *    	* TDC_TIME3
 *    	* ....
 *    	* TDC_TIME6
 *    	* TDC_CLOCK_COUNT1
 *    	* ....
 *    	* TDC_CLOCK_COUNT5
 *    	* TDC_CALIBRATION1
 *    	* TDC_CALIBRATION2
 * Return:
 *     >>
 *
 *============================================================*/
void DrvTdcGetResult(tdc_handle_t * handle, uint32_t *result);

/*============================================================
 * Function: DrvTdcEnterAutoMode
 * Usage: Enter DMA Auto Mode. After calling this function, the
 *
 * Para:
 *    >> tdc_handle_t: tdc handle.
 * Return:
 *    >>
 *
 *============================================================*/
void DrvTdcEnterAutoMode(tdc_handle_t * handle);

 /*============================================================
  * Function: DrvTdcStart()
  * Description: Start the measurement. This should be used:
  * (1) in non-DMA mode: to start a measure.
  * (2) in DMA mode: before start DMA, used to initialize the
  * 					transfer.
  * Para:
  *    >> tdc_handle_t * handle: a tdc7201 handle
  * Return:
  *     >>
  *
  *============================================================*/
void DrvTdcStart(tdc_handle_t * handle);


 /*============================================================
  * Function: DrvTdcEnable()
  * Usage: Enable the TDC720, Pull up the enable pin
  * Para:
  *    >> tdc_handle_t * handle: a tdc7201 handle
  * Return:
  *     >>
  *
  *============================================================*/
void DrvTdcEnable(tdc_handle_t * handle);


 /*============================================================
  * Function: TdcDisable()
  * Usage: Disable the TDC7201, pull down the enable pin
  * Para:
  *    >> tdc_handle_t * handle: a tdc7201 handle
  * Return:
  *     >>
  *
  *============================================================*/
void DrvTdcDisable(tdc_handle_t * handle);

/*============================================================
 * Function: DrvTdcSetComm
 * Description: Set communication handle
 *
 * Para:
 *    >> tdc_handle_t : a tdc7201 handle
 *    >> void: a communication handle
 * Return:
 *     >>
 * Change Record:
 *		>> (07/Fev/2018): Creation of the function;
 *
 *============================================================*/
void DrvTdcSetComm(tdc_handle_t * handle, void * comm);

/*============================================================
  * Function: DrvTdcNextChannel
  * Description: Set to next channel
  *
  * Para:
  *    >> tdc_handle_t : a tdc7201 handle
  * Return:
  *     >>
  * Change Record:
  *		>> (01/Mar/2018): Creation of the function;
  *
  *============================================================*/
 void DrvTdcNextChannel(tdc_handle_t * handle);


#endif /* BSP_DRIVERS_TDC_TDC_H_ */
