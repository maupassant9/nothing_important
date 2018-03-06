/*
 * ad7265.h
 *
 *  Created on: 21 de fev de 2018
 *      Author: dong.x
 */

#ifndef BSP_DRIVERS_AD7265_AD7265_H_
#define BSP_DRIVERS_AD7265_AD7265_H_


/*============================================================
 * Function:  DrvMcp3021Init
 * Description:  Initiate the dev and data structure
 * the channel chain configuration in conf is ignored because
 * MCP3021 has only one channel.
 * Para:
 *    >> adc_handle_t :  an adc handle.
 *    >> adc_conf_t :  an adc configuration structure.
 * Return:
 *     >>
 * Change Record:
 *		>> (07/Jan/2018): Creation of the function;
 *
 *============================================================*/
 void DrvAd7265Init(adc_handle_t * handle, void * conf);


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
 void DrvAd7265Link(adc_handle_t * handle, void * comm);

 /*============================================================
  * Function:  DrvMcp3021Start
  * Description:  Start conversion, mcp3021 start conversion is
  * initialized when soc get result through i2c, this function
  * will get the result and save it in a memory area in internal
  * memory.
  * Para:
  *    >> adc_handle_t :  an adc handle.
  * Return:
  *     >>
  * Change Record:
  *		>> (07/Jan/2018): Creation of the function;
  *
  *============================================================*/
 void DrvAd7265Start(adc_handle_t * handle);

 /*============================================================
  * Function:  DrvMcp3021Enable
  * Description: Enable the MCP3021, not supported, no effect
  * Para:
  *    >> adc_handle_t :  an adc handle.
  * Return:
  *     >>
  * Change Record:
  *		>> (07/Jan/2018): Creation of the function;
  *
  *============================================================*/
 void DrvAd7265NextChannel(adc_handle_t * handle);


 /*============================================================
  * Function:  DrvMcp3021GetResult
  * Description: Return the value saved in ptr_internal->conv.
  * Para:
  *    >> adc_handle_t :  an adc handle.
  * Return:
  *     >>
  * Change Record:
  *		>> (07/Jan/2018): Creation of the function;
  *
  *============================================================*/
 void DrvAd7265GetResult(adc_handle_t * handle, uint32_t *val);

 void DrvAd7265NotSupported(adc_handle_t * handle);



#endif /* BSP_DRIVERS_AD7265_AD7265_H_ */
