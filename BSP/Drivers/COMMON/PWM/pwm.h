/*
 * bsp_board.h
 *
 *  Created on: 27 de nov de 2017
 *  Author: dong.x
 *  Change Records:
 *     >>(12/Dec/2017): add uart_conf, UartInit();
 *     >>(13/Dec/2017): configuration struture added.
 *     >>(20/Dec/2017): finished.
 */

#ifndef BSP_DRIVERS_COMMON_PWM_PWM_H_
#define BSP_DRIVERS_COMMON_PWM_PWM_H_

#include "../BSP/type.h"
#include "../BSP/connector.h"

/*============================================================
 * Macro
 *============================================================*/

/*============================================================
 *Typedef
 *============================================================*/


/*============================================================
 *Function Prototype
 *============================================================*/

/*============================================================
 * Function: DrvPwmInit()
 * Para:
 *    >> pwm_handle_t * handle: a pwm handle
 *    >> pwm_conf_t * ptr: pwm configuration structure
 * Return:
 *     >>
 *  Usage: Initilization of pwm module. The first function
 * should be called before any other pwm function.
 * Change Record:
 *		>> (20/Dec/2017): Creation of the function;
 *
 *============================================================*/
void DrvPwmInit(pwm_handle_t * handle, pwm_conf_t * ptr_conf);


/*============================================================
 * Function: DrvPwmSetPulseWidth()
 * Para:
 *    >> pwm_handle_t * handle: a pwm handle
 *    >> pwm_conf_t * ptr: pwm configuration structure
 * Return:
 *     >> bool_t: true for sucessed, false for failed
 * Usage: Call this function for run-time change of pulse width
 * the new value should be valid after the counter reaches it's
 * maximum value.
 * Change Record:
 *		>> (20/Dec/2017): Creation of the function;
 *
 *============================================================*/
bool_t DrvPwmSetPulseWidth(pwm_handle_t * handle, uint32_t pulse_width);

/*============================================================
 * Function: DrvPwmSetDelay()
 * Para:
 *    >> pwm_handle_t * handle: a pwm handle
 *    >> pwm_conf_t * ptr: pwm configuration structure
 * Return:
 *     >> bool_t: true for sucessed, false for failed
 * Usage: Call this function for run-time change delay value,
 * the new value should be valid after the counter reaches it's
 * maximum value.
 * Use PWM0 with output pin EPWM0B.
 * Change Record:
 *		>> (20/Dec/2017): Creation of the function;
 *
 *============================================================*/
bool_t DrvPwmSetDelay(pwm_handle_t * handle, uint32_t delay);

/*============================================================
 * Function: DrvPwmSetFreq()
 * Para:
 *    >> pwm_handle_t * handle: a pwm handle
 *    >> pwm_conf_t * ptr: pwm configuration structure
 * Return:
 *     >> bool_t: true for sucessed, false for failed
 * Usage: Cal this function for run-time change of pwm frequency.
 * The new value should be valid after the counter reaches it's
 * maximum value. Frequency range: in_freq/clk_div/65535 <= f <= in_freq/2;
 * In case of in_freq = 150MHz, clk_div = 1:  2.29 kHz <= f <= 75 MHz 
 * Change Record: 
 *		>> (20/Dec/2017): Creation of the function;
 *
 *============================================================*/
bool_t DrvPwmSetFreq(pwm_handle_t * handle, uint32_t freq);

/*============================================================
 * Function: DrvPwmStart()
 * Para:
 *    >> pwm_handle_t * handle: a pwm handle
 *    >> pwm_conf_t * ptr: pwm configuration structure
 * Return:
 *     >> bool_t: true for sucessed, false for failed
 * Usage: Call this to start the pwm.
 * Change Record:
 *		>> (20/Dec/2017): Creation of the function;
 *
 *============================================================*/
bool_t DrvPwmStart(pwm_handle_t * handle);

/*============================================================
 * Function: DrvPwmStop()
 * Para:
 *    >> pwm_handle_t * handle: a pwm handle
 *    >> pwm_conf_t * ptr: pwm configuration structure
 * Return:
 *     >> bool_t: true for sucessed, false for failed
 * Usage: Stop the Pwm module. 
 * Change Record: 
 *		>> (20/Dec/2017): Creation of the function;
 *
 *============================================================*/
bool_t DrvPwmStop(pwm_handle_t * handle);


#endif /* BSP_DRIVERS_COMMON_PWM_PWM_H_ */
