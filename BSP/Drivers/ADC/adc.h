/* 
* FileName: mcp3021.h
* Author: Dong Xia 
* This is head file of mcp3021.
* 
* Change Records: 
*      >> (06/Fev/2018): Creation of file 
* 
*/


#ifndef  BSP_DRIVERS_ADC_ADC_H_
#define  BSP_DRIVERS_ADC_ADC_H_
/********************************************
* Include 
********************************************/
#include "../BSP/type.h"
#include "../BSP/conf.h"
#include "../BSP/connector.h"

/********************************************
* Macro 
********************************************/


/********************************************
* Type definition 
********************************************/
// a data structure for channel
typedef struct adc_ch_struct_t{
    uint8_t channel_no;
    void * ptr_extra_info;
    struct adc_ch_struct_t * next;
}adc_ch_handle_t;

/********************************************
* Function prototype 
********************************************/

/*------------------------------------------------ 
* Funcname: DrvAdcChInit(); 
* Usage: Initiate the Channel structure for ADC
* Paras: 
*  >> adc_ch_handle_t : The handle of channel in ADC
*  >> uint8_t : Channel number
*  >> adc_ch_handle_t : the next channel
*  >> void * : extra information for channel.
* Return: 
*  >> : 
* Change Records: 
*  >> (07/Fev/2017): Create the function 
*----------------------------------------------*/ 
void DrvAdcChInit(adc_ch_handle_t * handle, 
                  uint8_t channel_no,
                  adc_ch_handle_t * next,
                  void * ptr_extra);




#endif /* BSP_DRIVERS_ADC_ADC_H_ */
