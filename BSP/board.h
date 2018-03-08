/*
 * board.h
 *
 *  Created on: 27 de nov de 2017
 *  Author: dong.x
 *  Change Records:
 *     >>(11/Dec/2017): add definition of gpio used on board;
 *     >>(11/Dec/2017): add pinmutex value.
 *     >>(02/Fev/2017): add board strucutre
 */

#ifndef BSP_BOARD_H_
#define BSP_BOARD_H_

#include "conf.h"
#include "connector.h"
/*============================================================
 * Macro
 *============================================================*/
/* ============= Definition of GPIO used on board ============ */
// Check for schemetic for more information.
// 1 - DIPSWITCH GPIO
#define SW_FEED_PIN 97 //R5
#define SW_CLK_PIN 98 //K4
#define SW_SERIAL_PIN 99 //J3

// 2 - ADC7265 GPIO
#define ADC1_CH_SEL0 96 //C11
#define ADC1_CH_SEL0_MUX_NUM 11
#define ADC1_CH_SEL0_MUX_VAL 0x00000008
#define ADC1_CH_SEL0_MUX_LOC 0x0000000f
#define ADC1_CH_SEL1 65 //E12
#define ADC1_CH_SEL1_MUX_NUM 10
#define ADC1_CH_SEL1_MUX_VAL 0x80000000
#define ADC1_CH_SEL1_MUX_LOC 0xf0000000
#define ADC1_CH_SEL2 66 //B11
#define ADC1_CH_SEL2_MUX_NUM 10
#define ADC1_CH_SEL2_MUX_VAL 0x08000000
#define ADC1_CH_SEL2_MUX_LOC 0x0f000000

// 3 - Syncronization GPIO
#define SYNC_IN_PIN 12 //A2
#define SYNC_OUT_PIN 11 //A3

// 4 -  Laco GPIO
    // 4.1 - Estado do Laco
#define LACO1_STATE_PIN 120 //P4 -- GPIO7[7]
#define LACO2_STATE_PIN 121 //W1
#define LACO3_STATE_PIN 122 //W2
#define LACO4_STATE_PIN 123 //W3
    //4.2 - LACO CTR
#define LACO1_STOP_PIN 141 //G3
#define LACO1_STOP_MUX_NUM 18
#define LACO1_STOP_MUX_VAL 0x00800000
#define LACO1_STOP_MUX_LOC 0x00f00000
#define LACO2_STOP_PIN 142 //J4
#define LACO2_STOP_MUX_NUM 18
#define LACO2_STOP_MUX_VAL 0x00080000
#define LACO2_STOP_MUX_LOC 0x000f0000
#define LACO3_STOP_PIN 143 //G2
#define LACO3_STOP_MUX_NUM 18
#define LACO3_STOP_MUX_VAL 0x00008000
#define LACO3_STOP_MUX_LOC 0x0000f000
#define LACO4_STOP_PIN 144 //G1
#define LACO4_STOP_MUX_NUM 18
#define LACO4_STOP_MUX_VAL 0x00000800
#define LACO4_STOP_MUX_LOC 0x00000f00

#define LACO1_EN_PIN 113 //U3
#define LACO1_EN_MUX_NUM 18
#define LACO1_EN_MUX_VAL 0x00000080
#define LACO1_EN_MUX_LOC 0x000000f0
#define LACO2_EN_PIN 114 //T1
#define LACO2_EN_MUX_NUM 18
#define LACO2_EN_MUX_VAL 0x00000008
#define LACO2_EN_MUX_LOC 0x0000000f
#define LACO3_EN_PIN 115 //T2
#define LACO3_EN_MUX_NUM 17
#define LACO3_EN_MUX_VAL 0x80000000
#define LACO3_EN_MUX_LOC 0xf0000000
#define LACO4_EN_PIN 116 //T3
#define LACO4_EN_MUX_NUM 17
#define LACO4_EN_MUX_VAL 0x08000000
#define LACO4_EN_MUX_LOC 0x0f000000


    //4.3 - LACO Capacitor select
#define LACO_CAP_SFTCLK_PIN 102 //P17
#define SFTCLK_PIN_MUX_NUM 16
#define SFTCLK_PIN_MUX_VAL 0x00000080
#define SFTCLK_PIN_MUX_LOC 0x000000f0
#define LACO_CAP_LCHCLK_PIN 101 //H3
#define LCHCLK_PIN_MUX_NUM 19
#define LCHCLK_PIN_MUX_VAL 0x00000800
#define LCHCLK_PIN_MUX_LOC 0x00000f00
#define LACO_CAP_SDI_PIN 108 //U17
#define SDI_PIN_MUX_NUM 13
#define SDI_PIN_MUX_VAL 0x00080000
#define SDI_PIN_MUX_LOC 0x000f0000
#define LACO_CAP_RST_PIN 105 //T15
#define RST_PIN_MUX_NUM 13
#define RST_PIN_MUX_VAL 0x80000000
#define RST_PIN_MUX_LOC 0xf0000000
#define LACO_CAP_OE_PIN 106 //U16
#define OE_PIN_MUX_NUM 13
#define OE_PIN_MUX_VAL 0x08000000
#define OE_PIN_MUX_LOC 0x0f000000


//DipSwitch pins
#define DS_SW_CLK_PIN 98//K4: GP6_1
#define DS_SW_CLK_PIN_MUX_NUM 19
#define DS_SW_CLK_PIN_MUX_VAL 0x00800000
#define DS_SW_CLK_PIN_MUX_LOC 0x00f00000

#define DS_SW_FEED_PIN 97 //R5 GP6_0
#define DS_SW_FEED_PIN_MUX_NUM 19
#define DS_SW_FEED_PIN_MUX_VAL 0x08000000
#define DS_SW_FEED_PIN_MUX_LOC 0x0f000000

#define DS_SW_SER_PIN 99 //J3 GP6_2
#define DS_SW_SER_PIN_MUX_NUM 19
#define DS_SW_SER_PIN_MUX_VAL 0x00080000
#define DS_SW_SER_PIN_MUX_LOC 0x000f0000
// 5 - I2C GPIO
#define I2C0_PINS_MUX_NUM 4
#define I2C0_PINS_MUX_VAL 0x00002200
#define I2C0_PINS_MUX_LOC 0x0000ff00

#define I2C1_PINS_MUX_NUM 4
#define I2C1_PINS_MUX_VAL 0x00440000
#define I2C1_PINS_MUX_LOC 0x00ff0000

// 6 - LEDs GPIO
#define LACO1_STATE_LED_PIN 120 //P4 -- GPIO7[7]
#define LACO1_STATE_PIN_MUX_NUM 17
#define LACO1_STATE_PIN_MUX_VAL 0x00000800
#define LACO1_STATE_PIN_MUX_LOC 0x00000f00

#define LACO2_STATE_LED_PIN 121 //W1 --
#define LACO2_STATE_PIN_MUX_NUM 17
#define LACO2_STATE_PIN_MUX_VAL 0x00000080
#define LACO2_STATE_PIN_MUX_LOC 0x000000f0

#define LACO3_STATE_LED_PIN 122 //W2
#define LACO3_STATE_PIN_MUX_NUM 17
#define LACO3_STATE_PIN_MUX_VAL 0x00000008
#define LACO3_STATE_PIN_MUX_LOC 0x0000000f

#define LACO4_STATE_LED_PIN 123 //W3
#define LACO4_STATE_PIN_MUX_NUM 16
#define LACO4_STATE_PIN_MUX_VAL 0x80000000
#define LACO4_STATE_PIN_MUX_LOC 0xf0000000

#define LACO1_ERR_LED_PIN 124 //V1
#define LACO1_ERR_PIN_MUX_NUM 16
#define LACO1_ERR_PIN_MUX_VAL 0x08000000
#define LACO1_ERR_PIN_MUX_LOC 0x0f000000

#define LACO2_ERR_LED_PIN 125 //V2
#define LACO2_ERR_PIN_MUX_NUM 16
#define LACO2_ERR_PIN_MUX_VAL 0x00800000
#define LACO2_ERR_PIN_MUX_LOC 0x00f00000

#define LACO3_ERR_LED_PIN 126 //V3
#define LACO3_ERR_PIN_MUX_NUM 16
#define LACO3_ERR_PIN_MUX_VAL 0x00080000
#define LACO3_ERR_PIN_MUX_LOC 0x000f0000

#define LACO4_ERR_LED_PIN 127 //U1
#define LACO4_ERR_PIN_MUX_NUM 16
#define LACO4_ERR_PIN_MUX_VAL 0x00008000
#define LACO4_ERR_PIN_MUX_LOC 0x0000f000

#define SYS_STATE_LED_PIN 128 //U2
#define SYS_STATE_PIN_MUX_NUM 16
#define SYS_STATE_PIN_MUX_VAL 0x00000800
#define SYS_STATE_PIN_MUX_LOC 0x00000f00
// 7 - TDC7201 GPIO
#define TDC1_CS1_PIN  //D16
#define TDC1_CS1_NUM 3
#define TDC1_CS2_PIN  //E17
#define TDC1_CS2_NUM 2

#define TDC1_EN_PIN 117 //R1
#define TDC1_EN_PIN_MUX_NUM 17
#define TDC1_EN_PIN_MUX_VAL 0x00800000
#define TDC1_EN_PIN_MUX_LOC 0x00f00000

#define TDC1_INT1_PIN 2 //C3
#define TDC1_INT1_PIN_MUX_NUM 1
#define TDC1_INT1_PIN_MUX_VAL 0x08000000
#define TDC1_INT1_PIN_MUX_LOC 0x0f000000
#define TDC1_INT2_PIN 3 //D4
#define TDC1_INT2_PIN_MUX_NUM 1
#define TDC1_INT2_PIN_MUX_VAL 0x00800000
#define TDC1_INT2_PIN_MUX_LOC 0x00f00000

#define TDC2_CS1_PIN  //D18
#define TDC2_CS1_NUM 5
#define TDC2_CS2_PIN  //C19
#define TDC2_CS2_NUM 4

#define TDC2_EN_PIN 118 //R2
#define TDC2_EN_PIN_MUX_NUM 17
#define TDC2_EN_PIN_MUX_VAL 0x00080000
#define TDC2_EN_PIN_MUX_LOC 0x000f0000

#define TDC2_INT1_PIN 4 //C5
#define TDC2_INT1_PIN_MUX_NUM 1
#define TDC2_INT1_PIN_MUX_VAL 0x00080000
#define TDC2_INT1_PIN_MUX_LOC 0x000f0000
#define TDC2_INT2_PIN 5 //C4
#define TDC2_INT2_PIN_MUX_NUM 1
#define TDC2_INT2_PIN_MUX_VAL 0x00008000
#define TDC2_INT2_PIN_MUX_LOC 0x0000f000

#define TDC_SPI_DIN_PIN  //C18
#define TDC_SPI_DOUT_PIN  //C16
#define TDC_SPI_SCLK_PIN  //D19

// 8 - MMC Detect pin
#define MMC_DETECT_PIN 100 //K3

// 9 - Watchdog feed pin
#define WATCHDOG_FEED_PIN 104 //W14
#define WATCHDOG_FEED_PIN_MUX_NUM 14
#define WATCHDOG_FEED_PIN_MUX_VAL 0x00000008
#define WATCHDOG_FEED_PIN_MUX_LOC 0x0000000f
// 10 - LAN Interrupt
#define LAN_INT_PIN 1 //E4

#define LACO_PWM_PIN_MUX_NUM 3
#define LACO_PWM_PIN_MUX_VAL 0x00000020
#define LACO_PWM_PIN_MUX_LOC 0x000000f0
/* ============= Definition of PIN MUX register Value ============ */
//The value is generated by Pin Multiplex Utility 1.0.1241.34.
#define PINMUX0_VAL 0x00880000
#define PINMUX1_VAL 0x88888000
#define PINMUX2_VAL null
#define PINMUX3_VAL 0x11111121
#define PINMUX4_VAL 0x22222288
#define PINMUX5_VAL 0x00010102
#define PINMUX6_VAL null
#define PINMUX7_VAL null
#define PINMUX8_VAL null
#define PINMUX9_VAL null
#define PINMUX10_VAL 0x88222222
#define PINMUX11_VAL 0x00000008
#define PINMUX12_VAL null
#define PINMUX13_VAL 0x88080000
#define PINMUX14_VAL 0x88888808
#define PINMUX15_VAL 0x00000088
#define PINMUX16_VAL 0x88888880
#define PINMUX17_VAL 0x88880888
#define PINMUX18_VAL 0x00888888
#define PINMUX19_VAL 0x08888800

//#ifdef DEBUG
//#define PIN_NUM(grp,no) grp*16+no+1
//#define LACO_CAP_SFTCLK_PIN PIN_NUM(6,4) //GP6_4
//#define SFTCLK_PIN_MUX_NUM 19
//#define SFTCLK_PIN_MUX_VAL 0x00000800
//#define SFTCLK_PIN_MUX_LOC 0x00000f00
//#define LACO_CAP_LCHCLK_PIN PIN_NUM(6,3) //GP6_3
//#define LCHCLK_PIN_MUX_NUM 19
//#define LCHCLK_PIN_MUX_VAL 0x00008000
//#define LCHCLK_PIN_MUX_LOC 0x0000f000
//#define LACO_CAP_SDI_PIN PIN_NUM(8,15) //GP8_15
//#define SDI_PIN_MUX_NUM 18
//#define SDI_PIN_MUX_VAL 0x00000800
//#define SDI_PIN_MUX_LOC 0x00000f00
//#define LACO_CAP_RST_PIN PIN_NUM(8,13) //GP8_13
//#define RST_PIN_MUX_NUM 18
//#define RST_PIN_MUX_VAL 0x00080000
//#define RST_PIN_MUX_LOC 0x000f0000
//#define LACO_CAP_OE_PIN PIN_NUM(6,2) //GP6_2
//#define OE_PIN_MUX_NUM 19
//#define OE_PIN_MUX_VAL 0x00080000
//#define OE_PIN_MUX_LOC 0x000f0000
//#endif

#define UART0_BAUDRATE 9600

#define SPI0_FREQ 20000000
#define SPI0_CS_PINS 0x3c
#define SPI1_FREQ 20000000
#define SPI1_CS_PINS 0x2

#define I2C_SPEED 400000
/*============================================================
 *Typedef
 *============================================================*/



/*============================================================
 *Function Prototype
 *============================================================*/

/*===================================
 * BoardInit()
 * Para:
 *    >> --
 * Return:
 *    >> --
 * Usage: Initilization of Board
 *===================================*/
board_handle_t * BoardInit(void);

/*==============\=====================
 * GPIOInit()
 * Para:
 *    >> --
 * Return:
 *    >> --
 * Usage: Initilization of GPIO, this is a overall
 * initialization. After call this function, all of
 * the GPIOs are ready to be used. This function
 * should be called before any other module be
 * inicialized, becasue it sets the PinMux register.
 *===================================*/
void GPIOInit(void);


#endif /* BSP_BOARD_H_ */
