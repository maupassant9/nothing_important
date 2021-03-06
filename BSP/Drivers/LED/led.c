/*
* FileName: led.c
* Author: Dong Xia
* This is source file of led.
*
* Change Records:
*      >> (30/Jan/2018): file created.
*
*/

/********************************************
* Include
********************************************/
#include "../BSP/type.h"
#include "../BSP/conf.h"
#include "../BSP/connector.h"
#include "../BSP/board.h"
#include <stdlib.h>

#include "hw_types.h"
#include "include/hw/soc_OMAPL138.h"
#include "hw_syscfg0_OMAPL138.h"
#include "include/armv5/omapl138/interrupt.h"
#include "include/psc.h"
#include "include/gpio.h"

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
/*============================================================
 * Function: DrvLedsInit()
 * Description: initiate the GPIO and PSC used by LEDs & data
 * structure of Leds.
 * Para:
 *    >> leds_handle_t: led handle
 * Return:
 *     >>
 * Change Record:
 *		>> (30/Jan/2018): Creation of the function;
 *
 *============================================================*/
 void DrvLedsInit(leds_handle_t * handle)
 {
	 uint32_t save_pin_mux;
 	//1 - Initiate gpios: LACO State and error pins
	//1.1 - set laco led state gpio mutex.
	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO1_STATE_PIN_MUX_NUM)) &
					  ~LACO1_STATE_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO1_STATE_PIN_MUX_NUM)) =
					 (LACO1_STATE_PIN_MUX_VAL | save_pin_mux);

	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO2_STATE_PIN_MUX_NUM)) &
					  ~LACO2_STATE_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO2_STATE_PIN_MUX_NUM)) =
					 (LACO2_STATE_PIN_MUX_VAL | save_pin_mux);

	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO3_STATE_PIN_MUX_NUM)) &
					  ~LACO3_STATE_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO3_STATE_PIN_MUX_NUM)) =
					 (LACO3_STATE_PIN_MUX_VAL | save_pin_mux);

	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO4_STATE_PIN_MUX_NUM)) &
					  ~LACO4_STATE_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO4_STATE_PIN_MUX_NUM)) =
					 (LACO4_STATE_PIN_MUX_VAL | save_pin_mux);

	//1.2 - set laco led ERR gpio mutex.
	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO1_ERR_PIN_MUX_NUM)) &
					  ~LACO1_ERR_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO1_ERR_PIN_MUX_NUM)) =
					 (LACO1_ERR_PIN_MUX_VAL | save_pin_mux);

	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO2_ERR_PIN_MUX_NUM)) &
					  ~LACO2_ERR_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO2_ERR_PIN_MUX_NUM)) =
					 (LACO2_ERR_PIN_MUX_VAL | save_pin_mux);

	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO3_ERR_PIN_MUX_NUM)) &
					  ~LACO3_ERR_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO3_ERR_PIN_MUX_NUM)) =
					 (LACO3_ERR_PIN_MUX_VAL | save_pin_mux);

	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO4_ERR_PIN_MUX_NUM)) &
					  ~LACO4_ERR_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO4_ERR_PIN_MUX_NUM)) =
					 (LACO4_ERR_PIN_MUX_VAL | save_pin_mux);

	//1.3 - set system state led mutex
	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(SYS_STATE_PIN_MUX_NUM)) &
					  ~SYS_STATE_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(SYS_STATE_PIN_MUX_NUM)) =
					 (SYS_STATE_PIN_MUX_VAL | save_pin_mux);

	//2 - set gpios to OUTPUT mode.
	GPIODirModeSet(SOC_GPIO_0_REGS,LACO1_STATE_LED_PIN,GPIO_DIR_OUTPUT);
	GPIODirModeSet(SOC_GPIO_0_REGS,LACO2_STATE_LED_PIN,GPIO_DIR_OUTPUT);
	GPIODirModeSet(SOC_GPIO_0_REGS,LACO3_STATE_LED_PIN,GPIO_DIR_OUTPUT);
	GPIODirModeSet(SOC_GPIO_0_REGS,LACO4_STATE_LED_PIN,GPIO_DIR_OUTPUT);
	GPIODirModeSet(SOC_GPIO_0_REGS,LACO1_ERR_LED_PIN,GPIO_DIR_OUTPUT);
	GPIODirModeSet(SOC_GPIO_0_REGS,LACO2_ERR_LED_PIN,GPIO_DIR_OUTPUT);
	GPIODirModeSet(SOC_GPIO_0_REGS,LACO3_ERR_LED_PIN,GPIO_DIR_OUTPUT);
	GPIODirModeSet(SOC_GPIO_0_REGS,LACO4_ERR_LED_PIN,GPIO_DIR_OUTPUT);
	GPIODirModeSet(SOC_GPIO_0_REGS,SYS_STATE_LED_PIN,GPIO_DIR_OUTPUT);

	//2.3 - Set gpios output as low.
	GPIOPinWrite(SOC_GPIO_0_REGS,LACO1_STATE_LED_PIN,1);
	GPIOPinWrite(SOC_GPIO_0_REGS,LACO2_STATE_LED_PIN,1);
	GPIOPinWrite(SOC_GPIO_0_REGS,LACO3_STATE_LED_PIN,1);
	GPIOPinWrite(SOC_GPIO_0_REGS,LACO4_STATE_LED_PIN,1);
	GPIOPinWrite(SOC_GPIO_0_REGS,LACO1_ERR_LED_PIN,1);
	GPIOPinWrite(SOC_GPIO_0_REGS,LACO2_ERR_LED_PIN,1);
	GPIOPinWrite(SOC_GPIO_0_REGS,LACO3_ERR_LED_PIN,1);
	GPIOPinWrite(SOC_GPIO_0_REGS,LACO4_ERR_LED_PIN,1);
	GPIOPinWrite(SOC_GPIO_0_REGS,SYS_STATE_LED_PIN,1);

	//Data structure init.
	handle->leds_state = 0x00;
 }


 /*============================================================
  * Function: DrvLedsUpdate()
  * Description: Update the state of LED. This function turn on
  * or off the Leds according to the value of
  * leds_handle->leds_state.
  * Para:
  *    >> leds_handle_t: led handle
  * Return:
  *     >>
  * Change Record:
  *		>> (30/Jan/2018): Creation of the function;
  *
  *============================================================*/
 void DrvLedsUpdate(leds_handle_t * handle)
 {
	 uint8_t cnt = 0;
	 uint16_t leds_state = ~(handle->leds_state)&0x01ff;
	 uint8_t pins_no[9];

	 pins_no[0] = LACO1_STATE_LED_PIN;
	 pins_no[1] = LACO2_STATE_LED_PIN;
	 pins_no[2] = LACO3_STATE_LED_PIN;
	 pins_no[3] = LACO4_STATE_LED_PIN;
	 pins_no[4] = LACO1_ERR_LED_PIN;
	 pins_no[5] = LACO2_ERR_LED_PIN;
	 pins_no[6] = LACO3_ERR_LED_PIN;
	 pins_no[7] = LACO4_ERR_LED_PIN;
	 pins_no[8] = SYS_STATE_LED_PIN;

	 //Update to led
	 for(; cnt < 9; cnt++){
		GPIOPinWrite(SOC_GPIO_0_REGS,
					 pins_no[cnt],leds_state&0x0001);
		leds_state = leds_state >> 1;
	 }
 }


 /*============================================================
  * Function: DrvLedsSet()
  * Description: Set the corresponding LED to on state.
  * This function only changes the value of leds_handle->leds_state
  * If an immediate change is needed, a function call of
  * DrvLedsUpdate is preferred afterwards.
  * Para:
  *    >> leds_handle_t: led handle
  *    >> uint8_t: bit location in leds_state
  * Return:
  *     >>
  * Change Record:
  *		>> (30/Jan/2018): Creation of the function;
  *
  *============================================================*/
 void DrvLedsSet(leds_handle_t * handle, uint8_t bit_loc)
 {
	 //TODO: need lock here.
	 handle->leds_state = handle->leds_state | BIT(bit_loc);
 }

 /*============================================================
  * Function: DrvLedsUnset()
  * Description: Set the corresponding LED to off state.
  * This function only changes the value of leds_handle->leds_state
  * If an immediate change is needed, a function call of
  * DrvLedsUpdate is preferred afterwards.
  * Para:
  *    >> leds_handle_t: led handle
  *    >> uint8_t: bit location in leds_state
  * Return:
  *     >>
  * Change Record:
  *		>> (30/Jan/2018): Creation of the function;
  *
  *============================================================*/
 void DrvLedsUnset(leds_handle_t * handle, uint8_t bit_loc)
 {
	 handle->leds_state = handle->leds_state & ~BIT(bit_loc);
 }
