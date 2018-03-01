
/*
* FileName: led.h
* Author: Dong Xia
* This is head file of led.
*
* Change Records:
*      >> (29/Dec/2017): Creation of file
*
*/
#ifndef BSP_DRIVERS_LED_LED_H_
#define BSP_DRIVERS_LED_LED_H_
/********************************************
* Include
********************************************/


/********************************************
* Macro
********************************************/
#define LED_ESTADO_LACO1 0x01
#define LED_ESTADO_LACO2 0x02
#define LED_ESTADO_LACO3 0x03
#define LED_ESTADO_LACO4 0x04
#define LED_ERR_LACO1 0x05
#define LED_ERR_LACO2 0x06
#define LED_ERR_LACO3 0x07
#define LED_ERR_LACO4 0x08
#define LED_ESTADO_SYS 0x09

/********************************************
* Type definition
********************************************/


/********************************************
* Function prototype
********************************************/
/*============================================================
 * Function: DrvLedsInit()
 * Usage: Init func
 * Para:
 *    >>leds_handle_t: led handle
 * Return:
 *     >>
 * Change Record:
 *		>> (30/Jan/2018): Creation of the function;
 *
 *============================================================*/
 void DrvLedsInit(leds_handle_t * handle);

 /*============================================================
  * Function: DrvLedsUpdate()
  * Usage: Update the state of LED. This function turn on
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
 void DrvLedsUpdate(leds_handle_t * handle);

 /*============================================================
  * Function: DrvLedsSet()
  * Usage: Set the corresponding LED to on state.
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
 void DrvLedsSet(leds_handle_t * handle, uint8_t bit_loc);

 /*============================================================
  * Function: DrvLedsUnset()
  * Usage: Set the corresponding LED to off state.
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
 void DrvLedsUnset(leds_handle_t * handle, uint8_t bit_loc);




#endif /* BSP_DRIVERS_LED_LED_H_ */
