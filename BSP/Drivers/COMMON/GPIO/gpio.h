/*
 * bsp_gpio.h
 *
 *  Created on: 24 de nov de 2017
 *  Author: dong.x
 *  Version: 0.1
 *
 *  Description:
 *  Definition of GPIO used in placa de pesagem.
 */

#ifndef BSP_BSP_GPIO_H_
#define BSP_BSP_GPIO_H_

/*****************************************************************************
**                       MACRO DEFINITION
*****************************************************************************/
/* Pin Multiplexing bit mask to select GP2[12] pin. */
#define PINMUX5_GPIO2_12_ENABLE    (SYSCFG_PINMUX5_PINMUX5_15_12_GPIO2_12  << \
                                    SYSCFG_PINMUX5_PINMUX5_15_12_SHIFT)



/*==============GPIO Naming=============*/
// LEDs
// Naming rules:
// LED_D(#)_BANK:
// the bank number of # LED in schematic.
// LED_D(#)_PIN
// the pin number of # LED in schematic.
#define LED_D6_BANK 2
#define LED_D6_PIN 12


/*=============Function Prototype===============*/
void BspGpioInit(void);
void gpioMutexCfg(void);
void delay(void);


#endif /* BSP_BSP_GPIO_H_ */
