/*
 * ad7265.c
 * author: Dong Xia
 *
 *
 *  Created on: 11 de dez de 2017
 *      Author: dong.x
 */


/********************************************
* Include
********************************************/
#include "../BSP/Drivers/common/spi/spi.h"
#include "../BSP/Drivers/ADC/adc.h"
#include "hw_syscfg0_OMAPL138.h"
#include "include/hw/soc_OMAPL138.h"
#include "../BSP/board.h"
#include "include/gpio.h"
#include "ad7265.h"
#include "hw_types.h"
#include <stdlib.h>

/********************************************
* Internal Function Declaration
********************************************/


/********************************************
* Internal Types and Variables
********************************************/
extern board_handle_t * board;

typedef struct {
	adc_ch_handle_t * ch1;
	adc_ch_handle_t * ch2;
	adc_ch_handle_t * ch3;
	adc_ch_handle_t * ch4;
	adc_ch_handle_t * ch5;
	adc_ch_handle_t * ch6;
	adc_ch_handle_t * curr_ch;
	spi_handle_t * spi_handle;
	uint16_t data[2];
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
 void DrvAd7265Init(adc_handle_t * handle, void * conf)
 {
    adc_internal_t * ptr_internal;
    spi_handle_t *spi_handle = handle->ptr_internal;
    uint32_t save_pin_mux;


    ptr_internal = (adc_internal_t *)malloc(sizeof(adc_internal_t));
    handle->ptr_internal = ptr_internal;
    ptr_internal->spi_handle = spi_handle;


    //1 - Initiate gpios: LACO State and error pins
   	//1.1 - set ADC Channel Select Pin
   	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(ADC1_CH_SEL0_MUX_NUM)) &
   					  ~ADC1_CH_SEL0_MUX_LOC);
   	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(ADC1_CH_SEL0_MUX_NUM)) =
   					 (ADC1_CH_SEL0_MUX_VAL | save_pin_mux);
   	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(ADC1_CH_SEL1_MUX_NUM)) &
   					  ~ADC1_CH_SEL1_MUX_LOC);
   	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(ADC1_CH_SEL1_MUX_NUM)) =
   					 (ADC1_CH_SEL1_MUX_VAL | save_pin_mux);
   	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(ADC1_CH_SEL2_MUX_NUM)) &
   					  ~ADC1_CH_SEL2_MUX_LOC);
   	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(ADC1_CH_SEL2_MUX_NUM)) =
   					 (ADC1_CH_SEL2_MUX_VAL | save_pin_mux);

   	GPIODirModeSet(SOC_GPIO_0_REGS,ADC1_CH_SEL0, GPIO_DIR_OUTPUT);
	GPIODirModeSet(SOC_GPIO_0_REGS,ADC1_CH_SEL1, GPIO_DIR_OUTPUT);
	GPIODirModeSet(SOC_GPIO_0_REGS,ADC1_CH_SEL2, GPIO_DIR_OUTPUT);
   	GPIOPinWrite(SOC_GPIO_0_REGS, ADC1_CH_SEL0,0);
   	GPIOPinWrite(SOC_GPIO_0_REGS, ADC1_CH_SEL1,0);
   	GPIOPinWrite(SOC_GPIO_0_REGS, ADC1_CH_SEL2,0);



   	//Configure the Channel structure
   	ptr_internal->ch1 = (adc_ch_handle_t *)malloc(sizeof(adc_ch_handle_t));
   	ptr_internal->ch2 = (adc_ch_handle_t *)malloc(sizeof(adc_ch_handle_t));
   	ptr_internal->ch3 = (adc_ch_handle_t *)malloc(sizeof(adc_ch_handle_t));
   	ptr_internal->ch4 = (adc_ch_handle_t *)malloc(sizeof(adc_ch_handle_t));
   	ptr_internal->ch5 = (adc_ch_handle_t *)malloc(sizeof(adc_ch_handle_t));
   	ptr_internal->ch6 = (adc_ch_handle_t *)malloc(sizeof(adc_ch_handle_t));
   	//link the channel: here ptr_extra_info will be the information of CHANNEL SEL
   	ptr_internal->ch1->channel_no = 1;
   	ptr_internal->ch1->next = ptr_internal->ch2;
   	ptr_internal->ch2->channel_no = 2;
   	ptr_internal->ch2->next = ptr_internal->ch3;
   	ptr_internal->ch3->channel_no = 3;
   	ptr_internal->ch3->next = ptr_internal->ch4;
   	ptr_internal->ch4->channel_no = 4;
   	ptr_internal->ch4->next = ptr_internal->ch5;
   	ptr_internal->ch5->channel_no = 5;
   	ptr_internal->ch5->next = ptr_internal->ch6;
   	ptr_internal->ch6->channel_no = 6;
   	ptr_internal->ch6->next = ptr_internal->ch1;

   	ptr_internal->curr_ch = ptr_internal->ch1;

   	spi_handle->SetCs(spi_handle,0x01);
 }



/*============================================================
 * Function:  DrvAd7265Start
 * Description:  Start conversion, AD7265 start S/H and conversion.
 * It is initialized when soc r/w through spi, this function
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
void DrvAd7265Start(adc_handle_t * handle)
{
	uint16_t res[2];

	adc_internal_t * ptr_internal =
			(adc_internal_t *)(handle->ptr_internal);

	spi_handle_t *spi_handle = ptr_internal->spi_handle;

	res[0] = 0; res[1] = 0;
	//spi_handle->SetCs(spi_handle,0x01);
	spi_handle->CsHold(spi_handle,1, 0x01); //TODO:
	spi_handle->Write(spi_handle,0x00);
	res[0] = spi_handle->Read(spi_handle);
	spi_handle->Write(spi_handle,0x00);
	res[1] = spi_handle->Read(spi_handle);
	spi_handle->CsHold(spi_handle,0,0x01); //TODO: changed here. CsHold function more params

	//TODO: need to check the value;
	ptr_internal->data[0] = res[0]>>2;
	ptr_internal->data[1] = res[1]>>2;
}

/*============================================================
 * Function:  DrvAd7265GetResult
 * Description: Return the value saved in ptr_internal->data.
 * Para:
 *    >> adc_handle_t :  an adc handle.
 *    >> uint32_t *: the data will be copied into this address.
 *    	 XXXX XXXX XXXX XXXX
 *    	 |<-CHA->| |<-CHB->|
 * Return:
 *     >>
 * Change Record:
 *		>> (07/Jan/2018): Creation of the function;
 *
 *============================================================*/
void DrvAd7265GetResult(adc_handle_t * handle, uint32_t * val)
{
	adc_internal_t * ptr_internal =
					(adc_internal_t *)(handle->ptr_internal);

	*val = (((uint32_t)ptr_internal->data[0]) << 16);
	*val += (uint32_t)ptr_internal->data[1];
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
void DrvAd7265NextChannel(adc_handle_t * handle)
{
	adc_internal_t * ptr_internal;

	ptr_internal = (adc_internal_t *)handle->ptr_internal;
	ptr_internal->curr_ch = ptr_internal->curr_ch->next;

	switch(ptr_internal->curr_ch->channel_no)
	{
		case 1:
			GPIOPinWrite(SOC_GPIO_0_REGS,ADC1_CH_SEL0,0);
			GPIOPinWrite(SOC_GPIO_0_REGS,ADC1_CH_SEL1,0);
			GPIOPinWrite(SOC_GPIO_0_REGS,ADC1_CH_SEL2,0);
			break;
		case 2:
			GPIOPinWrite(SOC_GPIO_0_REGS,ADC1_CH_SEL0,1);
			break;
		case 3:
			GPIOPinWrite(SOC_GPIO_0_REGS,ADC1_CH_SEL0,0);
			GPIOPinWrite(SOC_GPIO_0_REGS,ADC1_CH_SEL1,1);
			break;
		case 4:
			GPIOPinWrite(SOC_GPIO_0_REGS,ADC1_CH_SEL0,1);
			break;
		case 5:
			GPIOPinWrite(SOC_GPIO_0_REGS,ADC1_CH_SEL0,0);
			GPIOPinWrite(SOC_GPIO_0_REGS,ADC1_CH_SEL1,0);
			GPIOPinWrite(SOC_GPIO_0_REGS,ADC1_CH_SEL2,1);
			break;
		case 6:
			GPIOPinWrite(SOC_GPIO_0_REGS,ADC1_CH_SEL0,1);
			break;
	}
}
/*============================================================
 * Function:  DrvAd7265Disable
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
 * Function:  DrvAd7265EnterAutoMode
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
void DrvAd7265NotSupported(adc_handle_t * handle)
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
void DrvAd7265Link(adc_handle_t * handle, void * comm)
{
    handle->ptr_internal = comm;
}
