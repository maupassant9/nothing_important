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
#include "../BSP/Drivers/ADC/adc.h"
#include "mcp3021.h"
#include "hw_types.h"
#include <stdlib.h>

/********************************************
* Internal Function Declaration 
********************************************/
#define COMM(handle) (i2c_handle_t *)handle
#define INTERNAL(ptr) (adc_internal_t *)ptr
uint16_t Mcp3021Read(adc_handle_t * handle);

/********************************************
* Internal Types and Variables 
********************************************/
static adc_ch_handle_t mcp_ch;

typedef struct {
    //current channel in channel chain
    adc_ch_handle_t * curr_ch;
    // i2c handle for communication
    i2c_handle_t * comm;
    // current saved conversion result
    uint16_t conv_val;

} adc_internal_t;


/********************************************
* External Variables 
********************************************/


/********************************************
* Functions 
********************************************/
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
 void DrvMcp3021Init(adc_handle_t * handle, void * conf)
 {
    adc_internal_t * ptr_internal;
    i2c_handle_t * comm = handle->ptr_internal;
    
    handle->dev_id = 2;
    handle->ptr_internal = (adc_internal_t *)malloc(sizeof(adc_internal_t));
    ptr_internal = INTERNAL(handle->ptr_internal);
    
    ptr_internal->comm = comm;

    // init the channel handle chain
    DrvAdcChInit(&mcp_ch,0,&mcp_ch,NULL);
    ptr_internal->curr_ch = &mcp_ch;
 }

/*============================================================
 * Function:  Mcp3021Read
 * Description:  Read MCP3021.
 * the channel chain configuration in conf is ignored because
 * MCP3021 has only one channel.
 * Para:
 *    >> adc_handle_t :  an adc handle.
 * Return:
 *     >>
 * Change Record:
 *		>> (07/Jan/2018): Creation of the function;
 *
 *============================================================*/
uint16_t Mcp3021Read(adc_handle_t * handle)
{
    uint8_t rsv_val[2], tmp;
    uint16_t val;
    adc_internal_t * ptr_internal = INTERNAL(handle->ptr_internal);

    ptr_internal->comm->addr = MCP3021_ADDR;
    ptr_internal->comm->Read(ptr_internal->comm, &rsv_val[0],2);

    val = (uint16_t)rsv_val[0];
    val = (val << 6) + (rsv_val[1] >> 2);
    return val;
}

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
void DrvMcp3021Start(adc_handle_t * handle)
{
    adc_internal_t * ptr_internal = INTERNAL(handle->ptr_internal);
    ptr_internal->conv_val = Mcp3021Read(handle);
}

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
void DrvMcp3021GetResult(adc_handle_t * handle, uint32_t * val)
{
    adc_internal_t * ptr_internal = INTERNAL(handle->ptr_internal);
    *val = (int32_t)ptr_internal->conv_val;
}

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
/*============================================================
 * Function:  DrvMcp3021Disable
 * Description: Disable the MCP3021, not supported, no effect
 * Para:
 *    >> adc_handle_t :  an adc handle.
 * Return:
 *     >>
 * Change Record:
 *		>> (07/Jan/2018): Creation of the function;
 *
 *============================================================*/
/*============================================================
 * Function:  DrvMcp3021EnterAutoMode
 * Description: Enter automatic mode, using DMA
 * Not supported, and no effect
 * Para:
 *    >> adc_handle_t :  an adc handle.
 * Return:
 *     >>
 * Change Record:
 *		>> (07/Jan/2018): Creation of the function;
 *
 *============================================================*/
void DrvMcp3021NotSupported(adc_handle_t * handle)
{
    //not supported, no effect at all.
}


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
void DrvMcp3021Link(adc_handle_t * handle, void * comm)
{
    handle->ptr_internal = comm;
}
