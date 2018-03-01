/*
 * dipswitch.c
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
#include "hw_types.h"
#include <stdlib.h>

/********************************************
* Internal Function Declaration
********************************************/
static uint32_t hc165Read();
static void hc165WriteBit(uint8_t val);

/********************************************
* Internal Types and Variables
********************************************/
typedef struct ds_internal_t {
	//hc165 rd value
	uint32_t hc165_val;

} ds_internal_t;


/********************************************
* External Variables
********************************************/
uint8_t hc165_pins[2];

/********************************************
* Functions
********************************************/
/*============================================================
 * Function:  DrvDsInit
 * Description:  Initiate the dip switch and data structure
 * Para:
 *    >>  :  an adc handle.
 *    >>  :  an adc configuration structure.
 * Return:
 *     >>
 * Change Record:
 *		>> (07/Jan/2018): Creation of the function;
 *
 *============================================================*/
 void DrvDsInit(ds_handle_t * handle)
 {
	 //do nothing here
	 uint32_t save_pin_mux;
	 ds_internal_t * ptr_internal = (ds_internal_t *)malloc(sizeof(ds_internal_t));

	 handle->ds_internal = (void *)ptr_internal;

	 save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(DS_SW_CLK_PIN_MUX_NUM)) &
					  ~DS_SW_CLK_PIN_MUX_LOC);
	 HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(DS_SW_CLK_PIN_MUX_NUM)) =
					 (DS_SW_CLK_PIN_MUX_VAL | save_pin_mux);

	 save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(DS_SW_FEED_PIN_MUX_NUM)) &
	 					  ~DS_SW_FEED_PIN_MUX_LOC);
	 HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(DS_SW_FEED_PIN_MUX_NUM)) =
	 				 (DS_SW_FEED_PIN_MUX_VAL | save_pin_mux);
	 save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(DS_SW_SER_PIN_MUX_NUM)) &
						  ~DS_SW_SER_PIN_MUX_LOC);
	 HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(DS_SW_SER_PIN_MUX_NUM)) =
					 (DS_SW_SER_PIN_MUX_VAL | save_pin_mux);

	 GPIODirModeSet(SOC_GPIO_0_REGS,DS_SW_CLK_PIN,GPIO_DIR_OUTPUT);
	 GPIODirModeSet(SOC_GPIO_0_REGS,DS_SW_FEED_PIN,GPIO_DIR_OUTPUT);
	 GPIODirModeSet(SOC_GPIO_0_REGS,DS_SW_SER_PIN,GPIO_DIR_INPUT);
	 //2.3 - Set gpios output as low.
	 GPIOPinWrite(SOC_GPIO_0_REGS,DS_SW_CLK_PIN,0);
	 GPIOPinWrite(SOC_GPIO_0_REGS,DS_SW_FEED_PIN,0);
	 hc165_pins[0] = DS_SW_CLK_PIN;
	 hc165_pins[1] = DS_SW_FEED_PIN;

	 ptr_internal->hc165_val = 0;
	 handle->has_changed = true;
	 ptr_internal->hc165_val = hc165Read();
 }

 /*============================================================
  * Function:  DrvDsUpdate()
  * Description:  Update the value of DIPSwitch, if the value
  * changes, update the has_changed bit.
  * Para:
  *    >>  :
  * Return:
  *     >>
  * Change Record:
  *		>> (07/Jan/2018): Creation of the function;
  *
  *============================================================*/
 void DrvDsUpdate(ds_handle_t * handle)
 {
	 ds_internal_t * ptr_internal =
			 (ds_internal_t *)handle->ds_internal;
	 uint32_t val = 0;
	 val= hc165Read();

	 if(val != ptr_internal->hc165_val)
	 {
		 handle->has_changed = true;
		 ptr_internal->hc165_val = val;
		 return;
	 }
 }

 /*============================================================
  * Function:  DrvDsRead()
  * Description:  Read the switch value
  * Para:
  *    >> ds_handle_t * : dip switch data structure.
  * Return:
  *     >> uint32_t: switch value:
  *     |       |<-Reserved           |_______|<-DIP2
  *     XXXX XXXX XXXX XXXX XXXX XXXX XXXX XXXX
  *         DIP4->|_______| |_______|<-DIP3
  * Change Record:
  *		>> (07/Jan/2018): Creation of the function;
  *
  *============================================================*/
 uint32_t DrvDsRead(ds_handle_t * handle)
 {
	 return ((ds_internal_t *)handle->ds_internal)->hc165_val;
 }

  /*============================================================
   * Function:  hc165Read()
   * Description:  Initiate the dip switch and data structure
   * Para:
   *    >>  :  an adc handle.
   *    >>  :  an adc configuration structure.
   * Return:
   *     >>
   * Change Record:
   *		>> (22/Fev/2018): Creation of the function;
   *
   *============================================================*/
  static uint32_t hc165Read()
   {
	   uint8_t cnt = 0;
	   uint32_t rd_val = 0;
	   //update
	   hc165WriteBit(0x03);
	   hc165WriteBit(0x01);
	   hc165WriteBit(0x03);

	   //hc165WriteBit(0x02);
	   for(cnt = 24; cnt > 0; cnt--)
	   {
		   hc165WriteBit(0x03);
		   //get the bit
		   rd_val += (GPIOPinRead(SOC_GPIO_0_REGS,DS_SW_SER_PIN)<<cnt);
		   hc165WriteBit(0x02);
	   }
	   hc165WriteBit(0x03);

	   rd_val = rd_val >> 1;
	   //correct the position here
	   rd_val = ((rd_val&0xff00)>>8)+((rd_val&0xff)<<8)+(rd_val&0xff0000);
	   return rd_val;
   }

  /*============================================================
   * Function:  hc165WriteBit()
   * Description:  Write bit to CLK and SER line.
   * Para:
   *    >> uint8_t : The last two bits write to CLK (bit0)
   *    and SER (bit1).
   * Return:
   *     >>
   * Change Record:
   *		>> (22/Fev/2018): Creation of the function;
   *
   *============================================================*/
  static void hc165WriteBit(uint8_t val){
	   GPIOPinWrite(SOC_GPIO_0_REGS,hc165_pins[0],val&0x01);
	   GPIOPinWrite(SOC_GPIO_0_REGS,hc165_pins[1],(val&0x02)>>1);
	   //delay
   }


