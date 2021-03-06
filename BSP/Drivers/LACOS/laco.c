/*
* laco.c
*
*  Created on: 05 de Jan de 2018
*  Author: dong.x
*  Version: 0.1
* 
*  Description: Source file for Laco control. 
*  Include 74HC595 driver (internal functions).

* 
* 	Change Records: 
*      >> (30/Jan/2018):finished
* 
*/

/*=========================================
* Includes
*=========================================*/
#include <stdlib.h>

#include "../BSP/type.h"
#include "../BSP/conf.h"
#include "../BSP/connector.h"
#include "../BSP/board.h"
#include "hw_types.h"
#include "include/hw/soc_OMAPL138.h"
#include "hw_syscfg0_OMAPL138.h"
#include "include/armv5/omapl138/interrupt.h"
#include "include/psc.h"
#include "include/gpio.h"
#include "laco.h"


/********************************************
 * Internal Function Declaration
********************************************/
//Internal funcs for HC595
static void Hc595GpioWrite(unsigned char pin_number, unsigned char val);
static void Hc595GpiosWriteAll(unsigned char val);
static void Hc595Init(void);
static void Hc595Write(uint32_t val);
static void Hc595GpioInit(void);
static void LacoDisable(lacos_handle_t *handle);
static void LacoEnable(lacos_handle_t *handle);
#ifdef WAIT_CYCLE
static void Hc595Wait(unsigned char mini_seg);
#endif

/********************************************
 * Internal Types and Variables
 *********************************************/
//data structure used to control single laco
typedef struct laco_struct_t{
    uint8_t id; //not used.
    //enable pin number
    uint8_t en_pin_number;
    //intervalo of pwm wave.
    uint16_t len_interval; 
    struct laco_struct_t * next;
} laco_t;

//Internal data structure for internal use.
typedef struct {
    laco_t laco1;
    laco_t laco2;
    laco_t laco3;
    laco_t laco4;
    laco_t * current;
    //Value used to save current laco cap state
    // According to hardware, arrange caps_val as
    // follow:
    //           _laco4 _laco3 _laco2 _laco1
    //           XXXXXX XXXXXX XXXXXX XXXXXX
    //    CAP1-->|    |<--CAP6 
    // for example: in laco1 segment, the value
    // is 001110, this means that the CAP3,CAP4
    // and CAP5 are selected, and CAP1&2&6 are
    // not connected with the circuit.
    uint32_t caps_val;
    pwm_handle_t *pwm_handle;
} lacos_internal_t;

//data struct used to save pin numbers
//connected with 74HC595
//The list of pin_number, in the following order:
//*           GPIO_SER
//*           GPIO_SRCLK
//*           GPIO_RCLK
//*           GPIO_SRCLR
//*           GPIO_OE
static uint16_t hc595_pins[5];

/********************************************
 * External Variables
********************************************/


/********************************************
 * Functions
********************************************/

/*------------------------------------------------
 * DrvLacoInit();
 * Initialize Laco Caps select pin.
 * Paras:
 *  >> lacos_handle_t *: a laco handle
 * Return:
 *  >> 
 * Change Records:
 *  >> (30/Jan/2018): Create the function
 *----------------------------------------------*/
void DrvLacoInit(lacos_handle_t * handle)
{
    lacos_internal_t * ptr_internal;
    pwm_handle_t * pwm = (pwm_handle_t *)handle->ptr_internal;
    uint32_t save_pin_mux;

    //create internal structure
    ptr_internal = (lacos_internal_t *)malloc(sizeof(lacos_internal_t));
    handle->ptr_internal = ptr_internal;
    ptr_internal->pwm_handle = pwm;

    //1 - Initiate HC595 and set Caps value
    Hc595Init();
    Hc595Write(ptr_internal->caps_val);

    //2 - Initiate other gpios: LACO enable pins
    //2.1 - set gpio mutex.
    save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO1_EN_MUX_NUM)) &
	 				  ~LACO1_EN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO1_EN_MUX_NUM)) =
			 	 	 (LACO1_EN_MUX_VAL | save_pin_mux);

    save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO2_EN_MUX_NUM)) &
	 				  ~LACO2_EN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO2_EN_MUX_NUM)) =
			 	 	 (LACO2_EN_MUX_VAL | save_pin_mux);
    save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO3_EN_MUX_NUM)) &
	 				  ~LACO3_EN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO3_EN_MUX_NUM)) =
			 	 	 (LACO3_EN_MUX_VAL | save_pin_mux);
    save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO4_EN_MUX_NUM)) &
	 				  ~LACO4_EN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LACO4_EN_MUX_NUM)) =
			 	 	 (LACO4_EN_MUX_VAL | save_pin_mux);
    //2.2 - set gpios to OUTPUT mode.
    GPIODirModeSet(SOC_GPIO_0_REGS,LACO1_EN_PIN,GPIO_DIR_OUTPUT);
    GPIODirModeSet(SOC_GPIO_0_REGS,LACO2_EN_PIN,GPIO_DIR_OUTPUT);
    GPIODirModeSet(SOC_GPIO_0_REGS,LACO3_EN_PIN,GPIO_DIR_OUTPUT);
    GPIODirModeSet(SOC_GPIO_0_REGS,LACO4_EN_PIN,GPIO_DIR_OUTPUT);
    //2.3 - Set gpios output as low.
    GPIOPinWrite(SOC_GPIO_0_REGS,LACO1_EN_PIN,0);
    GPIOPinWrite(SOC_GPIO_0_REGS,LACO2_EN_PIN,0);
    GPIOPinWrite(SOC_GPIO_0_REGS,LACO3_EN_PIN,0);
    GPIOPinWrite(SOC_GPIO_0_REGS,LACO4_EN_PIN,0);

    //3 - Initiate the laco internal structure
    ptr_internal->laco1.id = 1;
    ptr_internal->laco1.en_pin_number = LACO1_EN_PIN;
    ptr_internal->laco1.next = &ptr_internal->laco2;
    ptr_internal->laco1.len_interval = handle->lacos_pwm_len[0];
    
    ptr_internal->laco2.id = 2;
    ptr_internal->laco2.en_pin_number = LACO2_EN_PIN;
    ptr_internal->laco2.next = &ptr_internal->laco3;
    ptr_internal->laco2.len_interval = handle->lacos_pwm_len[1];

    ptr_internal->laco3.id = 3;
    ptr_internal->laco3.en_pin_number = LACO3_EN_PIN;
    ptr_internal->laco3.next = &ptr_internal->laco4;
    ptr_internal->laco3.len_interval = handle->lacos_pwm_len[2];

    ptr_internal->laco4.id = 4;
    ptr_internal->laco4.en_pin_number = LACO4_EN_PIN;
    ptr_internal->laco4.next = &ptr_internal->laco1;
    ptr_internal->laco4.len_interval = handle->lacos_pwm_len[3];

    ptr_internal->current= &ptr_internal->laco1;

    DrvLacoSetCap(handle,1,handle->lacos_caps_val[0]);
    DrvLacoSetCap(handle,2,handle->lacos_caps_val[1]);
    DrvLacoSetCap(handle,3,handle->lacos_caps_val[2]);
    DrvLacoSetCap(handle,4,handle->lacos_caps_val[3]);
//    ptr_internal->caps_val = (handle->lacos_caps_val[0]&0x3f)<<18;
//    ptr_internal->caps_val += (handle->lacos_caps_val[1]&0x3f)<<12;
//    ptr_internal->caps_val += (handle->lacos_caps_val[2]&0x3f)<<6;
//    ptr_internal->caps_val += handle->lacos_caps_val[3]&0x3f;

    //TODO:Enable the laco1
}

/*============================================================
 * Function: LacoEnable
 * Description: Enable the current laco
 * Para:
 *    >> laco_handle :  handle of laco
 * Return:
 *     >>
 * Change Record:
 *		>> (30/Jan/2018): Creation of the function;
 *
 *============================================================*/
 static void LacoEnable(lacos_handle_t *handle)
 {
    lacos_internal_t * ptr_internal;

    ptr_internal = (lacos_internal_t *)handle->ptr_internal;
    GPIOPinWrite(SOC_GPIO_0_REGS,
                ptr_internal->current->en_pin_number,1);
 }

/*============================================================
 * Function: LacoDisable
 * Description: Disable the current laco
 * Para:
 *    >> laco_handle :  handle of laco
 * Return:
 *     >>
 * Change Record:
 *		>> (30/Jan/2018): Creation of the function;
 *
 *============================================================*/
 static void LacoDisable(lacos_handle_t *handle)
 {
    lacos_internal_t * ptr_internal;

    ptr_internal = (lacos_internal_t *)handle->ptr_internal;
    GPIOPinWrite(SOC_GPIO_0_REGS,
                ptr_internal->current->en_pin_number,0);
 }


/*============================================================
 * Function: DrvLacoNext
 * Description: Point to next laco, set pwm waveform length
 * for next laco.
 * Para:
 *    >> laco_handle :  handle of laco
 * Return:
 *     >>
 * Change Record:
 *		>> (30/Jan/2018): Creation of the function;
 *
 *============================================================*/
 void DrvLacoNext(lacos_handle_t *handle)
 {
    lacos_internal_t * ptr_internal;

    ptr_internal = (lacos_internal_t *)handle->ptr_internal;
    LacoDisable(handle);
    ptr_internal->current = ptr_internal->current->next;
    ptr_internal->pwm_handle->SetPulseWidth(ptr_internal->pwm_handle,
    						ptr_internal->current->len_interval);
    //TODO: enable the next laco??
    LacoEnable(handle);
 }


/*------------------------------------------------
 * DrvLacoSetCap();
 * Set Laco Caps select pin.
 * Paras:
 *  >> lacos_handle_t: handle for laco
 *  >> uint8_t: number of laco: laco1..laco4
 * Return:
 *  >> 
 * Change Records:
 *  >> (05/Jan/2018): Create the function
 *----------------------------------------------*/
void DrvLacoSetCap(lacos_handle_t * handle,
                   uint8_t laco_num,
				   uint8_t cap_val)
{
    lacos_internal_t *ptr_internal  =
            (lacos_internal_t *)handle->ptr_internal;
    uint32_t caps_val = ptr_internal->caps_val;

    cap_val = cap_val & 0x3f;

    switch(laco_num)
    {
        case 4: caps_val = (caps_val&0xffffffc0)|cap_val;
                Hc595Write(caps_val);
                ptr_internal->caps_val = caps_val; break;
        case 3: caps_val = (caps_val&0xfffff03f)|((uint32_t)cap_val<<6);
                Hc595Write(caps_val);
                ptr_internal->caps_val = caps_val; break;
        case 2: caps_val = (caps_val&0xfffc0fff)|((uint32_t)cap_val<<12);
                Hc595Write(caps_val);
                ptr_internal->caps_val = caps_val; break;
        case 1: caps_val = (caps_val&0xff03ffff)|((uint32_t)cap_val<<18);
                Hc595Write(caps_val);
                ptr_internal->caps_val = caps_val; break;
        default: break;
    }
}



/*------------------------------------------------
 * DrvLacoLink();
 * Link the laco and PWM
 * Paras:
 *  >> lacos_handle_t: handle for lacos
 *  >> pwm_handle_t: pwm_handle
 * Return:
 *  >> 
 * Change Records:
 *  >> (8/Fev/2018): Create the function
 *----------------------------------------------*/
void DrvLacoLink(lacos_handle_t * handle, pwm_handle_t *pwm_handle)
{
    handle->ptr_internal = (void*)pwm_handle;
}

/*------------------------------------------------
 * DrvLacoStart();
 * Start the pwm signal.
 * Paras:
 *  >> lacos_handle_t: handle for lacos
 * Return:
 *  >>
 * Change Records:
 *  >> (20/Fev/2018): Create the function
 *----------------------------------------------*/
void DrvLacoStart(lacos_handle_t * handle)
{
	lacos_internal_t * ptr_internal;
	ptr_internal = handle->ptr_internal;

	ptr_internal->pwm_handle->Start(ptr_internal->pwm_handle);

	ptr_internal->pwm_handle->SetPulseWidth(ptr_internal->pwm_handle,
	    						ptr_internal->current->len_interval);
	//TODO: enable the next laco??
	LacoEnable(handle);
}


//========Functions for 74HC595==========
/*------------------------------------------------
 * Hc595Init();
 * Initialize the 74HC595 Device.
 * Paras:
 *  >> 
 * Return:
 *  >> 
 * Change Records:
 *  >> (05/Jan/2018): Create the function
 *----------------------------------------------*/
static void Hc595Init(void)
{
    unsigned char ser = DEFAULT_VALUE;

    hc595_pins[0] = LACO_CAP_SDI_PIN;
    hc595_pins[1] = LACO_CAP_SFTCLK_PIN;
    hc595_pins[2] = LACO_CAP_LCHCLK_PIN;
    hc595_pins[3] = LACO_CAP_RST_PIN;
    hc595_pins[4] = LACO_CAP_OE_PIN;

    Hc595GpioInit();
    //Reset the 74HC595 into default mode
    Hc595GpiosWriteAll(0x18|ser);
    Hc595GpiosWriteAll(0x18|ser);
    Hc595GpiosWriteAll(0x10|ser);
    Hc595GpiosWriteAll(0x0c|ser);

    Hc595Write(0x00);
}

/*------------------------------------------------
 * Hc595GpiosWriteAll();
 * Write one bit to all the GPIOs
 * Paras:
 *  >> unsigned char: Set all the gpios state
 *           Bit0: GPIO_SER
 *           Bit1: GPIO_SRCLK
 *           Bit2: GPIO_RCLK
 *           Bit3: GPIO_SRCLR
 *           Bit4: GPIO_OE
 * Return:
 *  >> 
 * Change Records:
 *  >> (05/Jan/2018): Create the function
 *----------------------------------------------*/
static void Hc595GpiosWriteAll(unsigned char val)
{
    unsigned char cnt = 0;
    for(;cnt < 5; cnt++){
        Hc595GpioWrite(hc595_pins[cnt], val&0x01);
        val = val >> 1;
    }
    //wait for one freq
#ifdef WAIT_CYCLE
    Hc595Wait(WAIT_CYCLE);
#endif
}


/*------------------------------------------------
 * Hc595GpioWrite();
 * Write to one GPIO
 * Paras:
 *  >> unsigned char: the pin number that write to;
 *  >> unsigned char: a valor write to gpio
 * Return:
 *  >> 
 * Change Records:
 *  >> (05/Jan/2018): Create the function
 *----------------------------------------------*/
static void Hc595GpioWrite(unsigned char pin_number,
                        unsigned char val)
{
	GPIOPinWrite(SOC_GPIO_0_REGS, pin_number,val);
//    if(val == 1){
//        GPIOPinWrite(SOC_GPIO_0_REGS, pin_number,HIGH);
//    } else {
//        GPIOPinWrite(SOC_GPIO_0_REGS, pin_number,LOW);
//    }
}

/*------------------------------------------------
 * Hc595Write();
 * Write serial data (24 bits) to Hc595.
 *           Bit0: GPIO_SER
 *           Bit1: GPIO_SRCLK
 *           Bit2: GPIO_RCLK
 *           Bit3: GPIO_SRCLR
 *           Bit4: GPIO_OE
 * Check the waveform in /waveform/write.png
 * Paras:
 *  >> hc595_handle_t: a handle for 74HC595 dev
 *  >> unsigned char: a valor write to gpio
 * Return:
 *  >> 
 * Change Records:
 *  >> (05/Jan/2018): Create the function
 *----------------------------------------------*/
static void Hc595Write(uint32_t val)
{
    unsigned char ser, cnt;
    for(cnt = 0; cnt < 24; cnt++){
        ser = val&0x01;
        Hc595GpiosWriteAll(0x08|ser);
        Hc595GpiosWriteAll(0x0a|ser);
        val = val >> 1;
    }
    Hc595GpiosWriteAll(0x0c);
    Hc595GpiosWriteAll(0x08);        
}   


#ifdef WAIT_CYCLE
/*------------------------------------------------
 * Hc595Wait();
 * Wait for a while
 * Check the waveform in /waveform/write.png
 * Paras:
 *  >> unsigned char: the miniseconds that needs to
 *  wait
 * Return:
 *  >> 
 * Change Records:
 *  >> (05/Jan/2018): Create the function
 *----------------------------------------------*/
static void Hc595Wait(unsigned char wait_cyc)
{
    volatile int i = 0, j = 0;
    //usleep((long)mini_seg*1000);
    //TODO: wait for some while.
    for(;i < wait_cyc; i++){
        j++;
    }
}
#endif

/*------------------------------------------------
 * Hc595GpioInit();
 * Init the GPIO used by HC595
 * Paras:
 *  >> 
 * Return:
 *  >> 
 * Change Records:
 *  >> (05/Jan/2018): Create the function
 *----------------------------------------------*/
static void Hc595GpioInit(void)
{
    uint32_t save_pin_mux;
    save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(SFTCLK_PIN_MUX_NUM)) &
	 				  ~SFTCLK_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(SFTCLK_PIN_MUX_NUM)) =
			 	 	 (SFTCLK_PIN_MUX_VAL | save_pin_mux);
    save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LCHCLK_PIN_MUX_NUM)) &
	 				  ~LCHCLK_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(LCHCLK_PIN_MUX_NUM)) =
			 	 	 (LCHCLK_PIN_MUX_VAL | save_pin_mux);
    save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(SDI_PIN_MUX_NUM)) &
	 				  ~SDI_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(SDI_PIN_MUX_NUM)) =
			 	 	 (SDI_PIN_MUX_VAL | save_pin_mux);
    save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(RST_PIN_MUX_NUM)) &
	 				  ~RST_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(RST_PIN_MUX_NUM)) =
			 	 	 (RST_PIN_MUX_VAL | save_pin_mux);
    save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(OE_PIN_MUX_NUM)) &
	 				  ~OE_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(OE_PIN_MUX_NUM)) =
			 	 	 (OE_PIN_MUX_VAL | save_pin_mux);
    // set gpios to output
    GPIODirModeSet(SOC_GPIO_0_REGS,LACO_CAP_SFTCLK_PIN,GPIO_DIR_OUTPUT);
    GPIODirModeSet(SOC_GPIO_0_REGS,LACO_CAP_LCHCLK_PIN,GPIO_DIR_OUTPUT);
    GPIODirModeSet(SOC_GPIO_0_REGS,LACO_CAP_SDI_PIN,GPIO_DIR_OUTPUT);
    GPIODirModeSet(SOC_GPIO_0_REGS,LACO_CAP_RST_PIN,GPIO_DIR_OUTPUT);
    GPIODirModeSet(SOC_GPIO_0_REGS,LACO_CAP_OE_PIN,GPIO_DIR_OUTPUT);
}
