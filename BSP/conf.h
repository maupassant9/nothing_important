/*
 * type.h
 *
 *  Created on: 13 de Dec de 2017
 *  Author: dong.x
 *  Config file for BSP
 *  Change Records:
 *     >>(13/Dec/2017): creation of this file;
 *     >>
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/*======================Config: debug====================*/
// A switch for Debug
#define DEBUG
// A switch for DSP/ARM
//#define OMAPL138_DSP


/*=====================Config: Hardware====================*/
#define OSC_IN_FREQ 24000000 //45600000 //Input oscillator frequency

#define PLL0_MULTIPLE_FACTOR 19 //10 //input PLL multiple factor
#define PLL0_PRE_DIV 1
#define PLL0_POST_DIV 1
#define PLL0_DIV_1 1
#define PLL0_DIV_2 2
#define PLL0_DIV_3 3
#define PLL0_DIV_4 4
#define PLL0_DIV_5 3
#define PLL0_DIV_6 1
#define PLL0_DIV_7 6

#define PLL1_MULTIPLE_FACTOR 25 //input PLL multiple factor
#define PLL1_POST_DIV 2
#define PLL1_DIV_1 1
#define PLL1_DIV_2 2
#define PLL1_DIV_3 3
/*====================Config: Software====================*/
//Function used to lock the resource
#define LOCK(n) Lock(n)
#define UNLOCK(n) Unlock(n)

/*====================DO NOT CHANGE ANYTHING BELOW============*/
//==============Clock===============
// CPU running frequency
#define PLL0_FREQ OSC_IN_FREQ*PLL0_MULTIPLE_FACTOR/PLL0_PRE_DIV/PLL0_POST_DIV

#define ARM_FREQ PLL0_FREQ/PLL0_DIV_6
#define DSP_FREQ PLL0_FREQ/PLL0_DIV_1

#define PLL0_SYSCLK2_FREQ PLL0_FREQ/PLL0_DIV_2
#define EMIFA_FREQ PLL0_FREQ/PLL0_DIV_3
#define PLL0_SYSCLK4_FREQ PLL0_FREQ/PLL0_DIV_4
#define EMAC_RMII_FREQ PLL0_FREQ/PLL0_DIV_7

#define PLL1_FREQ OSC_IN_FREQ*PLL1_MULTIPLE_FACTOR/PLL1_POST_DIV
#define DDR2_PHY_FREQ PLL1_FREQ/PLL1_DIV_1
#define PLL1_SYSCLK2_FREQ PLL1_FREQ/PLL1_DIV_2
#define PLL0_REF_FREQ PLL1_FREQ/PLL1_DIV_3

// Peripheral Clock
#define UART_1_2_FREQ PLL1_SYSCLK2_FREQ //input clk freq for usart 1/2
#define UART_0_FREQ PLL0_SYSCLK2_FREQ

#define PWM_0_FREQ PLL1_SYSCLK2_FREQ
#define SPI_0_FREQ PLL0_SYSCLK2_FREQ
#define SPI_1_FREQ PLL1_SYSCLK2_FREQ

#define PLL0_AUXCLK_FREQ OSC_IN_FREQ

#define I2C_0_FREQ PLL0_AUXCLK_FREQ //freq of I2C0
#define I2C_1_FREQ PLL0_AUXCLK_FREQ //freq of I2C1

#endif
