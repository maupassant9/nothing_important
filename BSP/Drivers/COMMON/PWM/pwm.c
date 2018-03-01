/*
 * pwm.c
 *
 *  Created on: 18 de dec de 2017
 *  Author: dong.x
 *  Version: 0.1
 *
 *  Description: Source file for PWM module.
 *
 * 	Change Records:
 *      >> (20/Dec/2017): Creation of function: DrvPwmInit(),
 *                        CalcMaxCntVal, CalcCmpVal,
 *                        DrvPwmSetPulseWidth(), DrvPwmSetDelay(),
 *                        DrvPwmSetFreq().
 *      >> (21/Dec/2017): Finished, tested ok. DrvPwmSetFreq has
 *                        overflow problem (TBPRD), no need to 
 *                        correct it since we will not need any
 *                        frequency that cause this bug.
 */

/*=========================================
 * Includes
 *=========================================*/
#include "../BSP/type.h"
#include "../BSP/conf.h"
#include "../BSP/connector.h"
#include "../BSP/board.h"
#include <stdlib.h>

#include "include/ehrpwm.h"
#include "hw_types.h"
#include "include/hw/soc_OMAPL138.h"
#include "hw_syscfg0_OMAPL138.h"
#include "include/armv5/omapl138/interrupt.h"
#include "include/psc.h"
#include "pwm.h"

/*=========================================
 * Internal Variables
 *=========================================*/
//Internal data strucutre: will not be freed.
struct pwm_internal_paras{
    uint32_t max_cnter_val; //max counter value //TODO:Delete
    uint32_t cmp_a_val; //comparator A value
    uint32_t cmp_b_val; //comparator B value
    uint32_t in_freq_after_div; //input frequency after clk divider
    // the same value of freq, delay, pulse_width in pwm_conf_t
    uint32_t freq;
    uint32_t delay;
    uint32_t pulse_width;
    uint32_t clk_div;
};

typedef struct pwm_internal_paras pwm_internal_t;

/*=========================================
 * Internal function declaration
 *=========================================*/
static uint32_t CalcMaxCnterVal(uint32_t in_freq, uint32_t freq);
static uint32_t CalcCmpVal(uint32_t in_freq, uint32_t delay);
static void DelayPwm(void);
//void *malloc(int);

/*=========================================
 * Functions
 *=========================================*/

/*============================================================
 * Function: DrvPwmInit()
 * Description: Initilization of pwm module.
 * Use PWM0 with output pin EPWM0B.
 * Para:
 *    >> pwm_handle_t * handle: a pwm handle
 *    >> pwm_conf_t * ptr: pwm configuration structure
 * Return:
 *     >>
 * Change Record:
 *		>> (20/Dec/2017): Creation of the function;
 *
 *============================================================*/
void DrvPwmInit(pwm_handle_t * handle, pwm_conf_t * ptr_conf)
{
    pwm_internal_t * ptr_internal;
    uint32_t save_pin_mux;

    PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_EHRPWM,
                                PSC_POWERDOMAIN_ALWAYS_ON,
        		                PSC_MDCTL_NEXT_ENABLE);
    //Create the internal data structure
    handle->ptr_pwm_data = malloc(sizeof(pwm_internal_t));
    ptr_internal = (pwm_internal_t *) handle->ptr_pwm_data;
    DelayPwm();

    //Set PWM pin
    save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO_PWM_PIN_MUX_NUM)) &
    	 				  ~LACO_PWM_PIN_MUX_LOC);
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO_PWM_PIN_MUX_NUM)) =
    			 	 	 (LACO_PWM_PIN_MUX_VAL | save_pin_mux);


    //Set the internal data structure
    ptr_internal->in_freq_after_div = ptr_conf->in_freq/ptr_conf->clk_div;
    ptr_internal->max_cnter_val = CalcMaxCnterVal(
                        ptr_internal->in_freq_after_div,
                        ptr_conf->freq);//TODO:DELETE!
    ptr_internal->cmp_a_val = CalcCmpVal(
                        ptr_internal->in_freq_after_div,
                        ptr_conf->delay);
    ptr_internal->cmp_b_val = CalcCmpVal(
                        ptr_internal->in_freq_after_div,
                        ptr_conf->delay+
                        ptr_conf->pulse_width);
    ptr_internal->freq = ptr_conf->freq;
    ptr_internal->delay = ptr_conf->delay;
    ptr_internal->pulse_width = ptr_conf->pulse_width;
    ptr_internal->clk_div = ptr_conf->clk_div;


    //Set the clock divider
    EHRPWMTimebaseClkConfig(SOC_EHRPWM_0_REGS,
            PWM_0_FREQ/ptr_conf->clk_div,
            PWM_0_FREQ);
    //Configure the period of the pwm waveform
    EHRPWMPWMOpFreqSet(SOC_EHRPWM_0_REGS,
            (PWM_0_FREQ/ptr_conf->clk_div),
		    ptr_conf->freq, EHRPWM_COUNT_UP, //count up
            EHRPWM_SHADOW_WRITE_ENABLE); //write shadowed

    //Disable Syncout
    EHRPWMSyncOutModeSet(SOC_EHRPWM_0_REGS, EHRPWM_SYNCOUT_DISABLE);

    //Configure the emulation behavior
    EHRPWMTBEmulationModeSet(SOC_EHRPWM_0_REGS,
    		EHRPWM_FREE_RUN);

    //Set compare A value
    EHRPWMLoadCMPA(SOC_EHRPWM_0_REGS,
                   ptr_internal->cmp_a_val,
                   EHRPWM_SHADOW_WRITE_ENABLE,
		           EHRPWM_COMPA_LOAD_COUNT_EQUAL_PERIOD,
                   EHRPWM_CMPCTL_OVERWR_SH_FL);

    //Set compare B value
     EHRPWMLoadCMPB(SOC_EHRPWM_0_REGS,
                   ptr_internal->cmp_b_val,
                   EHRPWM_SHADOW_WRITE_ENABLE,
		           EHRPWM_COMPA_LOAD_COUNT_EQUAL_PERIOD,
                   EHRPWM_CMPCTL_OVERWR_SH_FL);

    //Configure Action Qualifier:
    // EPWM0A not used, set EPWM0B
    //(1) set when CTR = CA up
    //(2) clear when CTR = CB up
    EHRPWMConfigureAQActionOnB(SOC_EHRPWM_0_REGS,
                    EHRPWM_AQCTLA_ZRO_DONOTHING,
                    EHRPWM_AQCTLA_PRD_DONOTHING,
					EHRPWM_AQCTLA_CAU_EPWMXAHIGH,
                    EHRPWM_AQCTLA_CAD_DONOTHING,
					EHRPWM_AQCTLA_CBU_EPWMXALOW,
		            EHRPWM_AQCTLA_CBD_DONOTHING,
                    EHRPWM_AQSFRC_ACTSFA_DONOTHING);

    // Bypass dead band, chopper, and trip event
    EHRPWMDBOutput(SOC_EHRPWM_0_REGS,
                    EHRPWM_DBCTL_OUT_MODE_BYPASS);
    EHRPWMChopperDisable(SOC_EHRPWM_0_REGS);
    EHRPWMTZTripEventDisable(SOC_EHRPWM_0_REGS,
                    EHRPWM_TZ_ONESHOT);
    EHRPWMTZTripEventDisable(SOC_EHRPWM_0_REGS,
                    EHRPWM_TZ_CYCLEBYCYCLE);
    //Generate Event - No event generated.
    //default valor for register ETSEL
    //EHRPWMETIntSourceSelect(SOC_EHRPWM_1_REGS, EHRPWM_ETSEL_INTSEL_TBCTREQUCMPBINC);
}


/*============================================================
 * Function: DrvPwmSetPulseWidth()
 * Description: Set pulse width of waveform
 * Para:
 *    >> pwm_handle_t * handle: a pwm handle
 *    >> pwm_conf_t * ptr: pwm configuration structure
 * Return:
 *     >> bool_t: true for sucessed, false for failed
 * Change Record:
 *		>> (20/Dec/2017): Creation of the function;
 *
 *============================================================*/
bool_t DrvPwmSetPulseWidth(pwm_handle_t * handle, uint32_t pulse_width)
{
    pwm_internal_t * ptr_internal = (pwm_internal_t *)handle->ptr_pwm_data;

    ptr_internal->pulse_width = pulse_width;
    ptr_internal->cmp_b_val = CalcCmpVal(
                        ptr_internal->in_freq_after_div,
                        ptr_internal->delay + pulse_width);

    if(ptr_internal->cmp_b_val > ptr_internal->max_cnter_val)
    {
        return false; // error ocurred.
    }
    //Set the cmp B reg val
    EHRPWMLoadCMPB(SOC_EHRPWM_0_REGS,
                    ptr_internal->cmp_b_val,
                    EHRPWM_SHADOW_WRITE_ENABLE,
                    EHRPWM_COMPA_LOAD_COUNT_EQUAL_PERIOD,
                    EHRPWM_CMPCTL_OVERWR_SH_FL);
    return true;
}


/*============================================================
 * Function: DrvPwmSetDelay()
 * Description: Set delay value.
 * Use PWM0 with output pin EPWM0B.
 * Para:
 *    >> pwm_handle_t * handle: a pwm handle
 *    >> pwm_conf_t * ptr: pwm configuration structure
 * Return:
 *     >> bool_t: true for sucessed, false for failed
 * Change Record:
 *		>> (20/Dec/2017): Creation of the function;
 *
 *============================================================*/
bool_t DrvPwmSetDelay(pwm_handle_t * handle, uint32_t delay)
{
    pwm_internal_t * ptr_internal = (pwm_internal_t *)handle->ptr_pwm_data;

    ptr_internal->delay = delay;
    ptr_internal->cmp_a_val = CalcCmpVal(
                        ptr_internal->in_freq_after_div,
                        delay);
    ptr_internal->cmp_b_val = CalcCmpVal(
                        ptr_internal->in_freq_after_div,
                        delay+
						ptr_internal->pulse_width);

    if(ptr_internal->cmp_b_val > ptr_internal->max_cnter_val)
    {
        return false; // error ocurred.
    }
    //Set the cmp a reg val
    EHRPWMLoadCMPA(SOC_EHRPWM_0_REGS,
            ptr_internal->cmp_a_val,
            EHRPWM_SHADOW_WRITE_ENABLE,
            EHRPWM_COMPA_LOAD_COUNT_EQUAL_PERIOD,
            EHRPWM_CMPCTL_OVERWR_SH_FL);
    //Set the cmp B reg val
    EHRPWMLoadCMPB(SOC_EHRPWM_0_REGS,
            ptr_internal->cmp_b_val,
            EHRPWM_SHADOW_WRITE_ENABLE,
            EHRPWM_COMPA_LOAD_COUNT_EQUAL_PERIOD,
            EHRPWM_CMPCTL_OVERWR_SH_FL);
    return true;
}

/*============================================================
 * Function: DrvPwmSetFreq()
 * Description: Set the frequency of PWM wave.
 * Para:
 *    >> pwm_handle_t * handle: a pwm handle
 *    >> pwm_conf_t * ptr: pwm configuration structure
 * Return:
 *     >> bool_t: true for sucessed, false for failed
 * Change Record:
 *		>> (20/Dec/2017): Creation of the function;
 *
 *============================================================*/
bool_t DrvPwmSetFreq(pwm_handle_t * handle, uint32_t freq)
{
    pwm_internal_t * ptr_internal = (pwm_internal_t *)handle->ptr_pwm_data;

    ptr_internal->freq = freq;
    ptr_internal->max_cnter_val = CalcMaxCnterVal(
                        ptr_internal->in_freq_after_div,
                        ptr_internal->freq);//TODO:DELETE!

    if(ptr_internal->cmp_b_val > ptr_internal->max_cnter_val)
    {
        return false;
    }

    //Configure the period of the pwm waveform
    EHRPWMPWMOpFreqSet(SOC_EHRPWM_0_REGS,
            PWM_0_FREQ/ptr_internal->clk_div,
		    ptr_internal->freq, EHRPWM_COUNT_UP, //count up
            EHRPWM_SHADOW_WRITE_ENABLE); //write shadowed

    return true;
}


/*============================================================
 * Function: DrvPwmStart()
 * Description: Start the PWM module.
 * Para:
 *    >> pwm_handle_t * handle: a pwm handle
 *    >> pwm_conf_t * ptr: pwm configuration structure
 * Return:
 *     >> bool_t: true for sucessed, false for failed
 * Change Record:
 *		>> (20/Dec/2017): Creation of the function;
 *
 *============================================================*/
bool_t DrvPwmStart(pwm_handle_t * handle)
{
	uint32_t saved_reg;
	//TODO: tomorrow finish this
	saved_reg = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_CFGCHIP1);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_CFGCHIP1) = (0x000001000 | saved_reg);
	return true;
}

/*============================================================
 * Function: DrvPwmStop()
 * Description: Initilization of pwm module.
 * Use PWM0 with output pin EPWM0B.
 * Para:
 *    >> pwm_handle_t * handle: a pwm handle
 *    >> pwm_conf_t * ptr: pwm configuration structure
 * Return:
 *     >> bool_t: true for sucessed, false for failed
 * Change Record:
 *		>> (20/Dec/2017): Creation of the function;
 *
 *============================================================*/
bool_t DrvPwmStop(pwm_handle_t * handle)
{
    //TODO: tomorrow finish this
	uint32_t saved_reg;
	//TODO: tomorrow finish this
	saved_reg = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_CFGCHIP1);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_CFGCHIP1) = (~0x000001000)&saved_reg;
	return true;
}

/*============================================================
 * Function: CalcMaxCnterVal()
 * Description: Calculate the counter's Maximum value, which
 * corresponde to the TBPRD register.
 * Para:
 *    >> uint32_t in_freq: epwm input frequency
 *    >> uint32_t freq: frequency of pwm waveform
 * Return:
 *     >> uint32_t: return the TBPRD register number.
 *                  0: if parameter error occurred.
 * Change Record:
 *		>> (20/Dec/2017): Creation of the function;
 *
 *============================================================*/
static uint32_t CalcMaxCnterVal(uint32_t in_freq, uint32_t freq)
{
    uint32_t max_cnt_val = in_freq/freq;

    //O valor menor que 100, não é preciso.
    if(max_cnt_val < 100) return 0;
    return max_cnt_val-1;
}


/*============================================================
 * Function: CalcCmpVal()
 * Description: Calculate the comparator's value, which
 * corresponde to the CMPA or CMPB register.
 * Para:
 *    >> uint32_t in_freq: epwm input frequency.
 *    >> uint32_t delay: delay in ns.
 * Return:
 *     >> uint32_t: return the CMPA or CMPB register number.
 * Change Record:
 *		>> (20/Dec/2017): Creation of the function;
 *
 *============================================================*/
static uint32_t CalcCmpVal(uint32_t in_freq, uint32_t delay)
{
    uint32_t comp_val = (uint32_t)((double)in_freq/1000000000.0*delay);

    return comp_val;
}

static void DelayPwm(void)
{
	volatile uint32_t cnt = 0;

	for(cnt = 0; cnt < 0xffff; cnt++);
}
