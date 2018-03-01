
/* 
* FileName: laco.h
* Author: Dong Xia 
* This is head file of laco.h
* 
* Change Records: 
*      >> (29/Dec/2017): Creation of file 
* 
*/

#ifndef BSP_DRIVERS_LACOS_LACO_H_
#define BSP_DRIVERS_LACOS_LACO_H_
/********************************************
* Include 
********************************************/


/********************************************
* Macro 
********************************************/
#define HIGH 0x01 //high volts
#define LOW 0x00  //low volts
//Default value (HIGH or LOW) for all the output of 74HC595
#define DEFAULT_VALUE HIGH
//The delay cycle, larger value more delay time
//#define WAIT_CYCLE 0

/********************************************
* Type definition 
********************************************/


/********************************************
* Function prototype 
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
void DrvLacoInit(lacos_handle_t * handle);

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
 void DrvLacoNext(lacos_handle_t *handle);

/*------------------------------------------------
 * DrvLacoSetCap();
 * Set Laco Caps select pin.
 * Paras:
 *  >> lacos_handle_t: handle for laco
 *  >> uint8_t: number of laco: laco1..laco4
 *  >> uint8_t: cap_val, value of capacitor
 * Return:
 *  >>
 * Change Records:
 *  >> (05/Jan/2018): Create the function
 *----------------------------------------------*/
void DrvLacoSetCap(lacos_handle_t * handle,
                   uint8_t laco_num,
				   uint8_t cap_val);



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
void DrvLacoLink(lacos_handle_t * handle, pwm_handle_t *pwm_handle);


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
void DrvLacoStart(lacos_handle_t * handle);

#endif

