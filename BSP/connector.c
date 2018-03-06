/*
 * connector.c
 *
 *  Created on: 14 de dec de 2017
 *  Author: dong.x
 *  Version: 0.1
 *
 *  Description: Connector function for all the drivers.
 *
 * 	Change Records:
 *      >> (14/Dec/2017): file created;
 *                        add function: connectUart, connectorInit.
 *      >> (21/Dec/2017): change uart_handle to uartA_handle and
 *                        add uartB_handle.
 *      >> (09/Fev/2017): add more functions ConnectXXX();
 */

/*=========================================
 * Includes
 *=========================================*/
#include "type.h"
#include "conf.h"
#include "../BSP/Drivers/UART/uart.h"
#include "../BSP/Drivers/COMMON/pwm/pwm.h"
#include "../BSP/Drivers/COMMON/spi/spi.h"
#include "../BSP/Drivers/tdc/tdc.h"
#include "../BSP/Drivers/led/led.h"
#include "../BSP/Drivers/lacos/laco.h"
#include "../BSP/Drivers/COMMON/I2C/i2c.h"
#include "../BSP/Drivers/ADC/MCP3021/mcp3021.h"
#include "../BSP/Drivers/ADC/AD7265/ad7265.h"
#include "../BSP/Drivers/Watchdog/watchdog.h"
#include "../BSP/Drivers/DIPSWITCH/dipswitch.h"
#include "connector.h"
#include "board.h"
#include <stdlib.h>

/*=========================================
 * External variables
 *=========================================*/


#ifndef OMAPL138_DSP /* ARM */
watchdog_handle_t watchdog_handle;
#else /* DSP */
//should be keeped in shared memory

#endif


/*=========================================
 * Internal function declaration
 *=========================================*/
static void ConnectUart(uart_handle_t *);
static void ConnectPwm(pwm_handle_t *);
static void ConnectSpi(spi_handle_t *);
static void ConnectTdc(tdc_handle_t *);
static void ConnectLeds(leds_handle_t *);
static void ConnectAdc2(adc_handle_t * adc);
static void ConnectLacos(lacos_handle_t * handle);
static void ConnectAdc1(adc_handle_t * adc);
static void ConnectI2C(i2c_handle_t * i2c);
static void ConnectWatchdog(watchdog_handle_t * wtdog);
static void ConnectDipswitch(ds_handle_t * ds);

/*=========================================
 * Functions
 *=========================================*/

/*============================================================
 * Function: ConnectInit()
 * Description: Connect the Usart Driver and the connctor
 * This function should be changed when other driver is used.
 * Para:
 *      >>
 * Return:
 *      >>
 * Change Record:
 *		>> (14/Dec/2017): func created
 *      >> (20/Dec/2017): func modified, add func call ConnectPWM
 *
 *============================================================*/
void ConnectorInit(board_handle_t *board)
{
    pwm_handle_t * pwm0;
    spi_handle_t * spi0, *spi1;
    i2c_handle_t * i2c;
    spi_conf_t spi_conf;
    i2c_conf_t i2c_conf;
    pwm_conf_t pwm_conf;

    //Create uart0 and uart1 handle
    board->uart0 = (uart_handle_t *)malloc(sizeof(uart_handle_t));
    //board->uart1 = (uart_handle_t *)malloc(sizeof(uart_handle_t));
    ConnectUart(board->uart0);
    //ConnectUart(board->uart1);

    //Create 6 tdc handle
    board->tdc = (tdc_handle_t *)malloc(sizeof(tdc_handle_t));
//    board->tdc2 = (tdc_handle_t *)malloc(sizeof(tdc_handle_t));
//    board->tdc3 = (tdc_handle_t *)malloc(sizeof(tdc_handle_t));
//    board->tdc4 = (tdc_handle_t *)malloc(sizeof(tdc_handle_t));
    board->adc1 = (adc_handle_t *)malloc(sizeof(adc_handle_t));
    board->adc2 = (adc_handle_t *)malloc(sizeof(adc_handle_t));
    ConnectTdc(board->tdc);
//    ConnectTdc(board->tdc2);
//    ConnectTdc(board->tdc3);
//    ConnectTdc(board->tdc4);
    ConnectAdc1(board->adc1);
    ConnectAdc2(board->adc2);

    board->leds = (leds_handle_t *)malloc(sizeof(leds_handle_t));
    ConnectLeds(board->leds);

    board->watchdog = (watchdog_handle_t *) malloc(sizeof(watchdog_handle_t));
    ConnectWatchdog(board->watchdog);

    board->lacos = (lacos_handle_t *)malloc(sizeof(lacos_handle_t));
    ConnectLacos(board->lacos);

    board->dipswitch = (ds_handle_t *)malloc(sizeof(ds_handle_t));
    ConnectDipswitch(board->dipswitch);

	spi_conf.id = 0;
	spi_conf.clk_freq = SPI0_FREQ;
	spi_conf.clk_polarity_phase = 1;
	spi_conf.cs_delay = 0;
	spi_conf.cs_pins = SPI0_CS_PINS;
	spi_conf.data_length = 16;
	spi_conf.shift_lsb_first = false;
    spi0 = (spi_handle_t *)malloc(sizeof(spi_handle_t));
    ConnectSpi(spi0);
    spi0->Init(spi0,&spi_conf);

    //TODO:NEED to Change configuration of SPI1
    spi_conf.id = 1;
	spi_conf.clk_freq = SPI1_FREQ;
	spi_conf.clk_polarity_phase = 3;
	spi_conf.cs_delay = 0;
	spi_conf.cs_pins = SPI1_CS_PINS;
	spi_conf.data_length = 16;
	spi_conf.shift_lsb_first = false;
    spi1 = (spi_handle_t *)malloc(sizeof(spi_handle_t));
    ConnectSpi(spi1);
    spi1->Init(spi1,&spi_conf);

    i2c_conf.id = 0;
    i2c_conf.speed = I2C_SPEED;
    i2c = (i2c_handle_t *)malloc(sizeof(i2c_handle_t));
    ConnectI2C(i2c);
    i2c->Init(i2c,&i2c_conf);

    board->tdc->Link(board->tdc, (void *)spi0);
//    board->tdc2->Link(board->tdc2, (void *)spi0);
//    board->tdc3->Link(board->tdc3, (void *)spi0);
//    board->tdc4->Link(board->tdc4, (void *)spi0);
    board->adc1->Link(board->adc1, (void *)spi1);
    board->adc2->Link(board->adc2, (void *)i2c);

    pwm_conf.in_freq = PWM_0_FREQ;
    pwm_conf.freq = 16000;
    pwm_conf.delay = 1000;
    pwm_conf.clk_div = 1;
    pwm_conf.pulse_width = 200;
    pwm0 = (pwm_handle_t *)malloc(sizeof(pwm_handle_t));
    ConnectPwm(pwm0);
    pwm0->Init(pwm0,&pwm_conf);
    board->lacos->Link(board->lacos, pwm0);


}


/*============================================================
 * Function: ConnectUart()
 * Description: Connect the Uart Driver and the connctor
 * This function should be changed when other driver is used.
 * Para:
 *      >> uart_handle_t *: a uart handle that needs to be 
 *              connected with uart driver.
 * Return:
 *      >>
 * Change Record:
 *		>> (14/Dec/2017): function created;
 *      >> (16/Dec/2017): Complete function;
 *      >> (20/Dec/2017): change to uartA_handle
 *
 *============================================================*/
static void ConnectUart(uart_handle_t * uart)
{
    //Connect Uart 1
    uart->Init = DrvUartInit;
    uart->ReadChar = DrvUartReadChar;
    uart->ReadCharBlock = DrvUartReadCharBlock;
    uart->WriteCharBlock = DrvUartWriteCharBlock;
    uart->WriteChar = DrvUartWriteChar;
    uart->HasChar = DrvUartHasChar;
    uart->IsTxRdy = DrvUartIsTxRdy;
    uart->Disable = DrvUartDisable;
    uart->WriteString = DrvUartWriteStr;
}


/*============================================================
 * Function: ConnectPwm()
 * Description: Connect the Uart Driver and the connctor
 * This function should be changed when other driver is used.
 * Para:
 *      >>
 * Return:
 *      >>
 * Change Record:
 *		>> (20/Dec/2017): Func created;
 *
 *============================================================*/
static void ConnectPwm(pwm_handle_t * pwm)
{
    pwm->Init = DrvPwmInit;
    pwm->SetPulseWidth = DrvPwmSetPulseWidth;
    pwm->SetDelay = DrvPwmSetDelay;
    pwm->SetFreq = DrvPwmSetFreq;
    pwm->Start = DrvPwmStart;
    pwm->Stop = DrvPwmStop;
}

/*============================================================
 * Function: ConnectSpi()
 * Description: Connect the Spi Driver and the connctor
 * This function should be changed when other driver is used.
 * Para:
 *      >> 
 * Return:
 *      >>
 * Change Record:
 *		>> (21/Dec/2017): Func created;
 *
 *============================================================*/
static void ConnectSpi(spi_handle_t * spi)
{
    spi->Init = DrvSpiInit;
    spi->Read = DrvSpiRead;
    spi->Write = DrvSpiWrite;
    spi->SetCs = DrvSpiSetCs;
    spi->CsHold = DrvSpiCsHold;
}

/*============================================================
 * Function: ConnectTdc()
 * Description: Connect the tdc Driver and the connctor
 * This function should be changed when other driver is used.
 * Para:
 *      >> tdc_handle_t * : a tdc handle that needs to be 
 *                    connected with tdc driver.
 * Return:
 *      >>
 * Change Record:
 *		>> (21/Dec/2017): Func created;
 *
 *============================================================*/
static void ConnectTdc(tdc_handle_t * tdc){

	//Connect tdc0
	tdc->Init = DrvTdcInit;
	tdc->Start = DrvTdcStart;
	tdc->GetResult = DrvTdcGetResult;
	tdc->Enable = DrvTdcEnable;
	tdc->Disable = DrvTdcDisable;
	tdc->EnterAutoMode = DrvTdcEnterAutoMode;
    tdc->Link = DrvTdcSetComm;
    tdc->Next = DrvTdcNextChannel;
}


/*============================================================
 * Function: ConnectLeds()
 * Description: Connect the Leds Driver and the connector
 * This function should be changed when other driver is used.
 * Para:
 *      >> leds_handle_t *: a led handle that needs to be 
 *              be connected with driver
 * Return:
 *      >>
 * Change Record:
 *		>> (30/Jan/2017): Func created;
 *
 *============================================================*/
static void ConnectLeds(leds_handle_t * leds){
	leds->Init = DrvLedsInit;
	leds->Set = DrvLedsSet;
	leds->Clr = DrvLedsUnset;
	leds->Update = DrvLedsUpdate;
}

/*============================================================
 * Function: ConnectI2C()
 * Description: Connect the Leds Driver and the connector
 * This function should be changed when other driver is used.
 * Para:
 *      >> 
 * Return:
 *      >>
 * Change Record:
 *		>> (30/Jan/2017): Func created;
 *
 *============================================================*/
static void ConnectI2C(i2c_handle_t * i2c)
{
    i2c->Init = DrvI2cInit;
    i2c->Read = DrvI2cRead;
}


/*============================================================
 * Function: ConnectAdc1()
 * Description: Connect the ADC Driver: AD7253
 * This function should be changed when other driver is used.
 * Para:
 *      >> adc_handle_t * : adc handle that needs to be
 *                    connected with adc driver.
 * Return:
 *      >>
 * Change Record:
 *		>> (30/Jan/2017): Func created;
 *
 *============================================================*/
static void ConnectAdc1(adc_handle_t * adc)
{
    adc->Init = DrvAd7265Init;
    adc->Start = DrvAd7265Start;
    adc->GetResult = DrvAd7265GetResult;
    adc->Disable = DrvAd7265NotSupported;
    adc->Enable = DrvAd7265NotSupported;
    adc->EnterAutoMode = DrvAd7265NotSupported;
    adc->Link = DrvAd7265Link;
    adc->Next = DrvAd7265NextChannel;
}


/*============================================================
 * Function: ConnectAdc2()
 * Description: Connect the ADC Driver MCP3021
 * This function should be changed when other driver is used.
 * Para:
 *      >> adc_handle_t * : adc handle that needs to be
 *                    connected with adc driver.
 * Return:
 *      >>
 * Change Record:
 *		>> (30/Jan/2017): Func created;
 *
 *============================================================*/
static void ConnectAdc2(adc_handle_t * adc)
{
    adc->Init = DrvMcp3021Init;
    adc->Start = DrvMcp3021Start;
    adc->GetResult = DrvMcp3021GetResult;
    adc->Disable = DrvMcp3021NotSupported;
    adc->Enable = DrvMcp3021NotSupported;
    adc->EnterAutoMode = DrvMcp3021NotSupported;
    adc->Link = DrvMcp3021Link;
}

/*============================================================
 * Function: ConnectLacos()
 * Description: Connect the Lacos driver
 * This function should be changed when other driver is used.
 * Para:
 *      >> adc_handle_t * : adc handle that needs to be
 *                    connected with adc driver.
 * Return:
 *      >>
 * Change Record:
 *		>> (30/Jan/2017): Func created;
 *
 *============================================================*/
static void ConnectLacos(lacos_handle_t * handle)
{
    handle->Init = DrvLacoInit;
    handle->Start = DrvLacoStart;
    handle->Next = DrvLacoNext;
    handle->SetCaps = DrvLacoSetCap;
    handle->Link = DrvLacoLink;
}

/*============================================================
 * Function: ConnectWatchdog()
 * Description: Connect the watchdog driver
 * This function should be changed when other driver is used.
 * Para:
 *      >> watchdog_handle_t * : watchdog handle that needs to be
 *                    connected with watchdog driver.
 * Return:
 *      >>
 * Change Record:
 *		>> (30/Jan/2017): Func created;
 *
 *============================================================*/
static void ConnectWatchdog(watchdog_handle_t * wtdog){
	wtdog->Init = DrvWatchDogInit;
	wtdog->GiveFood = DrvWatchDogGiveFood;
#ifndef OMAPL138_DSP
	wtdog->Feed = DrvWatchDogFeed;
#endif
}

/*============================================================
 * Function: ConnectDipswitch()
 * Description: Connect the Lacos driver
 * This function should be changed when other driver is used.
 * Para:
 *      >> adc_handle_t * : adc handle that needs to be
 *                    connected with adc driver.
 * Return:
 *      >>
 * Change Record:
 *		>> (30/Jan/2017): Func created;
 *
 *============================================================*/
static void ConnectDipswitch(ds_handle_t * handle)
{
	handle->Init = DrvDsInit;
	handle->Read = DrvDsRead;
	handle->Update = DrvDsUpdate;
}
