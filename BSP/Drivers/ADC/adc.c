/*
* FileName: 
* Author: Dong Xia
* This is source file of 
*
* Change Records:
*      >> (06/Fev/2018): File Created
*
*/

/********************************************
* Include 
********************************************/
#include "../BSP/Drivers/common/i2c/i2c.h"
#include "adc.h"

/********************************************
* Internal Function Declaration 
********************************************/



/********************************************
* Internal Types and Variables 
********************************************/



/********************************************
* External Variables 
********************************************/


/********************************************
* Functions 
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
                  void * ptr_extra)
{
    handle->channel_no = channel_no;
    handle->next = next;
    handle->ptr_extra_info = ptr_extra;
}

