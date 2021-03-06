/*
 * bsp_gpio.c
 *
 *  Created on: 24 de nov de 2017
 *  Author: dong.x
 *  Version: 0.1
 *
 *  Description: Source file for gpio, the initialization of
 *  GPIO was done before any function was called in this file.
 *  The Initialization function should be located in corresponding
 *  board file, because depending on specific board, the gpio 
 *  configuration is different.
 * 
 *  Change Records: 
 *      >> (11/Dec/2017): Creation of this file;
 *      >> 
 */

/*=========================================
 * Includes
 *=========================================*/
/* ================ StarterWare include ===================*/
#include "gpio.h"

#include "hw_types.h"
#include "include/hw/soc_OMAPL138.h"
#include "include/gpio.h"
#include "include/psc.h"
#include "hw_syscfg0_OMAPL138.h"

/* ================ BSP include ===================*/

/*=========================================
 * Internal Variables
 *=========================================*/


/*=========================================
 * Functions
 *=========================================*/
void BspGpioInit(void)
{
	//Power control the GPIO module
	PSCModuleControl(SOC_PSC_1_REGS,HW_PSC_GPIO,PSC_POWERDOMAIN_ALWAYS_ON,PSC_MDCTL_NEXT_ENABLE);
    //Set the PSC
    PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_EHRPWM,
                        PSC_POWERDOMAIN_ALWAYS_ON,
		                PSC_MDCTL_NEXT_ENABLE);
	delay();
	//Mutiplex control
	gpioMutexCfg();
	//Set GPIO Direction
	GPIODirModeSet(SOC_GPIO_0_REGS,45,GPIO_DIR_OUTPUT);
	//GPIODirModeSet(SOC_GPIO_0_REGS,139,GPIO_DIR_OUTPUT);~

	//Set here the gpio input with interrupt enabled
	GPIODirModeSet(SOC_GPIO_0_REGS,139,GPIO_DIR_INPUT);
	GPIOIntTypeSet(SOC_GPIO_0_REGS,139,GPIO_INT_TYPE_FALLEDGE);
	GPIOBankIntEnable(SOC_GPIO_0_REGS, 8);
}

void gpioMutexCfg()
{
	unsigned int savePinmux = 0;

	 /*
	 ** Clearing the bit in context and retaining the other bit values
	 ** in PINMUX5 register.
	 */
	 savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(5)) &
				  ~(SYSCFG_PINMUX5_PINMUX5_15_12));

	 /* Setting the pins corresponding to GP2[12] in PINMUX13 register.*/
	 HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(5)) =
		  (PINMUX5_GPIO2_12_ENABLE | savePinmux);


	 savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) &
	 				  ~0xf0000000);

	 /* Setting the pins corresponding to GP2[12] in PINMUX13 register.*/
	 HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) =
			 	 	 (0x80000000 | savePinmux);



//	 savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(3)) &
//	 					  ~(0x00000020));
//	 	 HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(3)) =
//	 			 (0x00000020|savePinmux);
	 HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(3)) = 0x00000020;


	 //here set gpio interrupt pin: gpio8_10
}

void delay()
{
	volatile long tmp = 0xfffff;
	for(; tmp >= 0; tmp--){

	}
}
