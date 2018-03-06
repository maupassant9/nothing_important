/*
 * board.c
 *
 *  Created on: 27 de nov de 2017
 *  Author: dong.x
 *  Version: 0.1
 * 
 *  Description: Source file for Board. Contains all the 
 *  necessary function for board access.
 * 
 * 	Change Records: 
 *      >> (11/Dec/2017): Creation of function: GPIOAllInit();
 *         				  Creation of function BoardInit();
 *      >> (02/Fev/2018): add XXXInit() functions.
 * 
 */

/*=========================================
 * Includes
 *=========================================*/
#include "connector.h"
#include "board.h"
#include "conf.h"
#include "type.h"
#include <string.h>
#include "../BSP/Drivers/common/gpio/gpio.h"
#include <stdlib.h>

static void Delay();
static void LacosInit(board_handle_t *board);
static void BoardPrintWelcome(board_handle_t *board);
static void TdcInit(board_handle_t * board);
static void AdcInit(board_handle_t * board);
static void UartInit(board_handle_t * board);
static void LedsShow(board_handle_t * board);
static void LacosInit(board_handle_t * board);
static void WatchdogInit(board_handle_t * board);
static void DipswitchInit(board_handle_t * board);
static void itoa(int value, char *str, int base);

//This variable should never be deleted
//as long as the board exist. And all the
//threads or tasks in OS should has access
//to this variable.
//board_handle_t * board;

/*=========================================
 * Functions
 *=========================================*/
/*============================================================
 * Function: BoardInit()
 * Description: Initilization of all of the components of the
 * board.
 * 1. Initialize GPIO module.
 * Change Record: 
 *		>> (11/Dec/2017): Creation of the function
 *============================================================*/
board_handle_t * BoardInit()
{
	board_handle_t *board;
	
	//1. Create Board structure
	board = (board_handle_t *)malloc(sizeof(board_handle_t));
	
	//2. Connector structure create and initiate
	ConnectorInit(board);

	//3 - Init components
		//3.0 - LED
	board->leds->Init(board->leds);
			//3.0.1 LED light
	LedsShow(board);
		//3.1 - uart
	UartInit(board);

		//3.2 - init tdcs
	TdcInit(board);
			//link tdcs
	
		//3.3 - init adcs
	AdcInit(board);
		//3.4 - init dipswitch
	DipswitchInit(board);

		//3.5 - init lacos
	LacosInit(board);
	
		//3.6 - Init watchdong
	WatchdogInit(board);

	BoardPrintWelcome(board);
	board->uart0->WriteString(board->uart0,"Board Init Success!\n");

	//Enter into sysBios
    asm(" mrc p15, #0, r0, c1, c0, #0\n\t"
    " orr r0, r0, #0x00002000\n\t"
    " mcr p15, #0, r0, c1, c0, #0\n\t");
    board->uart0->WriteString(board->uart0,"Enter into SYS/BIOS...\n");


//    board->adc1->Next(board->adc1);//CH 2
//    board->adc1->Next(board->adc1);//CH 3
//    board->adc1->Next(board->adc1);//CH 4
//    board->adc1->Next(board->adc1);//CH 5
//    board->adc1->Next(board->adc1);//CH 6
	return board;
}


/*============================================================
 * Function: GPIOInit()
 * Description: Initilization of all of the GPIO used on board 
 * 1. Power on the GPIO module
 * 2. Set the Pin Multiplexing control register, the value generated
 *    by TI PIN Multiplex Utility Ver 1.0.1241.34
 * 3. Set the GPIO direction
 * Paras:
 * 		>>
 * Return:
 * 		>>
 * Change Record: 
 *		>> (11/Dec/2017): Creation of the function
 *============================================================*/
// void GPIOInit()
// {
// 	//1.Power control the GPIO module
// 	PSCModuleControl(SOC_PSC_1_REGS,HW_PSC_GPIO,PSC_POWERDOMAIN_ALWAYS_ON,PSC_MDCTL_NEXT_ENABLE);
// 	//TODO: Here check the state of module power on.



// 	//2.Mutiplex Control
// 	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(0)) = PINMUX0_VAL
// 	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1)) = PINMUX1_VAL
// 	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(3)) = PINMUX3_VAL;
	
// 	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(4)) = PINMUX4_VAL;
// 	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(5)) = PINMUX5_VAL;

// 	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(10)) = PINMUX10_VAL;
// 	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) = PINMUX11_VAL;
// 	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) = PINMUX13_VAL;
// 	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(14)) = PINMUX14_VAL;
// 	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(15)) = PINMUX15_VAL;
// 	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(16)) = PINMUX16_VAL;
// 	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(17)) = PINMUX17_VAL;
// 	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) = PINMUX18_VAL;
// 	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(19)) = PINMUX19_VAL;

// 	//3.Set GPIOs Direction
// 	// *Check GPIO information on maunal: GPIODefinition.docx.
// 	GPIODirModeSet(SOC_GPIO_0_REGS,SW_FEED_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,SW_CLK_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,SW_SERIAL_PIN,GPIO_DIR_INPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,ADC1_CH_SEL0,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,ADC1_CH_SEL1,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,ADC1_CH_SEL2,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,SYNC_IN_PIN,GPIO_DIR_INPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,SYNC_OUT_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO1_STATE_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO2_STATE_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO3_STATE_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO4_STATE_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO1_STOP_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO2_STOP_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO3_STOP_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO4_STOP_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO1_PWM_EN_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO2_PWM_EN_PIN,GPIO_DIR_INPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO3_PWM_EN_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO4_PWM_EN_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO_CAP_SFTCLK_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO_CAP_LCHCLK_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO_CAP_SDI_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO_CAP_RST_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO_CAP_OE_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO1_ERR_LED_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO2_ERR_LED_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO3_ERR_LED_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LACO4_ERR_LED_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,SYS_STATE_LED_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,TDC1_EN_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,TDC1_INT1_PIN,GPIO_DIR_INPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,TDC1_INT2_PIN,GPIO_DIR_INPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,TDC2_EN_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,TDC2_INT1_PIN,GPIO_DIR_INPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,TDC2_INT2_PIN,GPIO_DIR_INPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,MMC_DETECT_PIN,GPIO_DIR_INPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,WATCHDOG_FEED_PIN,GPIO_DIR_OUTPUT);
// 	GPIODirModeSet(SOC_GPIO_0_REGS,LAN_INT_PIN,GPIO_DIR_INPUT);
// }


/*============================================================
 * Function: BoardPrintWelcome()
 * Description: Print the welcome information for debug,
 * This function should be called after the uart init ok
 * Paras:
 * 		>> board_handle_t: board handle
 * Return:
 * 		>>
 * Change Record: 
 *		>> (08/Fev/2018): Creation of the function
 *============================================================*/
static void BoardPrintWelcome(board_handle_t * board)
{
	board->uart0->WriteString(board->uart0, 
	"=====================================\n");
	board->uart0->WriteString(board->uart0,
	"=                                   =\n");
	board->uart0->WriteString(board->uart0,
	"=        W  E  L  C  O  M  E        =\n");
	board->uart0->WriteString(board->uart0,
	"=     PERKONS - SDIP - 2018         =\n");
	board->uart0->WriteString(board->uart0,
	"=                                   =\n");
	board->uart0->WriteString(board->uart0,
	"=====================================\n");
}

/*============================================================
 * Function: TdcInit()
 * Description: init the tdc
 * Paras:
 * 		>> board_handle_t: board handle
 * Return:
 * 		>>
 * Change Record: 
 *		>> (08/Fev/2018): Creation of the function
 *============================================================*/
static void TdcInit(board_handle_t * board)
{
	tdc_conf_t tdc_conf;
	// 1 - tdc 1
	tdc_conf.dev_id = 0;
	//tdc_conf.cs = 2;
	tdc_conf.ptr_spi_handle = NULL;
	tdc_conf.stop_number = 5;
	tdc_conf.is_rising_edge = true;
	tdc_conf.multi_cycle_aging = 1;
	board->tdc->Init(board->tdc,&tdc_conf);
	board->uart0->WriteString(board->uart0,"TDCs init okay!\n");
	board->uart0->WriteString(board->uart0,"Connection = SPI0\n");
	board->uart0->WriteString(board->uart0,"RISING EDGE = TRUE\n");
	board->uart0->WriteString(board->uart0,"MEASURE CYCLES = 1\n");
	board->uart0->WriteString(board->uart0,"NUM STOP = 1\n");
	board->uart0->WriteString(board->uart0,"CALI PERIODS = 2 (FIXED)\n");
}

/*============================================================
 * Function: UartInit()
 * Description: init the uart1 and uart2
 * Paras:
 * 		>> board_handle_t: board handle
 * Return:
 * 		>>
 * Change Record: 
 *		>> (08/Fev/2018): Creation of the function
 *============================================================*/
static void UartInit(board_handle_t * board)
{
	uart_conf_t uart_conf;
	//uart1 for debug
	uart_conf.uart_id = 1;
	uart_conf.baud_rate = UART0_BAUDRATE;
	uart_conf.input_freq = UART_1_2_FREQ;
	uart_conf.interrupt_en_bits = 0x08;
	uart_conf.data_len = 0;
	uart_conf.fifo_level = 3;
	uart_conf.priority_stop_bits = 6;
	uart_conf.over_sample_rate = 1;
	board->uart0->Init(board->uart0,&uart_conf);
	board->uart0->WriteString(board->uart0,"Uart RS232 init okay!\n");
	board->uart0->WriteString(board->uart0,"Uart RS232 configured as:\n");
	board->uart0->WriteString(board->uart0,"device = UART1\n");
	board->uart0->WriteString(board->uart0,"BAUDRATE = 9600\n");
	board->uart0->WriteString(board->uart0,"DATA LENGTH = 8\n");
	board->uart0->WriteString(board->uart0,"POLARITY = ODD\n");
	board->uart0->WriteString(board->uart0,"STOP BITS = 2\n");
	//485 TODO: not initialized.
	uart_conf.uart_id = 2;
	uart_conf.baud_rate = UART0_BAUDRATE;
	uart_conf.input_freq = UART_1_2_FREQ;
	uart_conf.interrupt_en_bits = 0x08;
	uart_conf.data_len = 0;
	uart_conf.fifo_level = 3;
	uart_conf.priority_stop_bits = 6;
	uart_conf.over_sample_rate = 1;
	//board->uart1->Init(board->uart1,&uart_conf);
	board->uart0->WriteString(board->uart0,"Uart 485 init okay!\n");
	board->uart0->WriteString(board->uart0,"Uart 485 configured as:\n");
	board->uart0->WriteString(board->uart0,"device = UART2\n");
	board->uart0->WriteString(board->uart0,"BAUDRATE = 9600\n");
	board->uart0->WriteString(board->uart0,"DATA LENGTH = 8\n");
	board->uart0->WriteString(board->uart0,"POLARITY = ODD\n");
	board->uart0->WriteString(board->uart0,"STOP BITS = 2\n");
	board->uart0->WriteString(board->uart0,"!!!!Uart 485 not inited!!!!\n");
}

/*============================================================
 * Function: AdcInit()
 * Description: init the uart1 and uart2
 * Paras:
 * 		>> board_handle_t: board handle
 * Return:
 * 		>>
 * Change Record: 
 *		>> (08/Fev/2018): Creation of the function
 *============================================================*/
static void AdcInit(board_handle_t * board)
{
	adc_conf_t adc_conf;
	uint32_t cnt, res;
	char str[10];

	board->uart0->WriteString(board->uart0, "Initializing ADC1: AD7265...\n");
	//adc_1 init
	adc_conf.dev_id = 1;
	board->adc1->Init(board->adc1,(void *)&adc_conf);
	for(cnt = 0; cnt < 6; cnt++)
	{
		board->adc1->Start(board->adc1);
		board->adc1->GetResult(board->adc1, &res);
		itoa((5000*(res>>16))>>13, str, 10);
		board->uart0->WriteString(board->uart0, "Val ADC(A) = ");
		board->uart0->WriteString(board->uart0, str);
		board->uart0->WriteString(board->uart0," mV\n");
		itoa((5000*(res&0xffff))>>13, str, 10);
		board->uart0->WriteString(board->uart0, "Val ADC(B) = ");
		board->uart0->WriteString(board->uart0, str);
		board->uart0->WriteString(board->uart0," mV\n");
		board->adc1->Next(board->adc1);
	}

	//adc_conf.dev_id = 1;
	//board->uart0->WriteString(board->uart0, "ADC1: AD7265 OK");

	board->uart0->WriteString(board->uart0, "Initializing ADC2: MCP3021...\n");
	//adc_2 init
	adc_conf.dev_id = 2;
	board->adc2->Init(board->adc2, (void *)&adc_conf);
	board->uart0->WriteString(board->uart0, "ADC2: MCP3021 OK\n");
    board->adc2->Start(board->adc2);
    board->adc2->GetResult(board->adc2, &res);
    board->uart0->WriteString(board->uart0,"Val = ");
    itoa((33000*res)>>10, str, 10);
    board->uart0->WriteString(board->uart0, str);
    board->uart0->WriteString(board->uart0," mV\n");

}



/*============================================================
 * Function: LedsShow()
 * Description: turn on and off the leds
 * Paras:
 * 		>> board_handle_t: board handle
 * Return:
 * 		>>
 * Change Record: 
 *		>> (08/Fev/2018): Creation of the function
 *============================================================*/
static void LedsShow(board_handle_t * board)
{
	int cnt;
	for(cnt = 0; cnt < 0x09; cnt++)
	{
		board->leds->Set(board->leds,cnt);
		board->leds->Update(board->leds);
		Delay();
		board->leds->Clr(board->leds,cnt);
		board->leds->Update(board->leds);
	}
	board->leds->leds_state = 0;
	board->leds->Update(board->leds);
}


/*============================================================
 * Function: LacosInit()
 * Description: Initiate the Lacos
 * Paras:
 * 		>> board_handle_t: board handle
 * Return:
 * 		>>
 * Change Record: 
 *		>> (08/Fev/2018): Creation of the function
 *============================================================*/
static void LacosInit(board_handle_t * board)
{
	board->lacos->lacos_pwm_len[0] = 200;
	board->lacos->lacos_pwm_len[1] = 300;
	board->lacos->lacos_pwm_len[2] = 400;
	board->lacos->lacos_pwm_len[3] = 500;
	board->lacos->lacos_caps_val[0] = 0x01;
	board->lacos->lacos_caps_val[1] = 0x01;
	board->lacos->lacos_caps_val[2] = 0x01;
	board->lacos->lacos_caps_val[3] = 0x01;
	board->lacos->Init(board->lacos);
	board->uart0->WriteString(board->uart0, "Laco Init OK\n");
	board->uart0->WriteString(board->uart0,"Laco1: CAP1 selected\n");
	board->uart0->WriteString(board->uart0,"Laco2: CAP1 selected\n");
	board->uart0->WriteString(board->uart0,"Laco3: CAP1 selected\n");
	board->uart0->WriteString(board->uart0,"Laco4: CAP1 selected\n");
}


/*============================================================
 * Function: WatchdogInit()
 * Description: Initiate the Lacos
 * Paras:
 * 		>> board_handle_t: board handle
 * Return:
 * 		>>
 * Change Record:
 *		>> (08/Fev/2018): Creation of the function
 *============================================================*/
static void WatchdogInit(board_handle_t * board)
{
	board->watchdog->Init(board->watchdog);
	board->uart0->WriteString(board->uart0, "Watchdog Init OK\n");
}


static void DipswitchInit(board_handle_t * board)
{
	char str[32];
	board->dipswitch->Init(board->dipswitch);
	board->uart0->WriteString(board->uart0,
			"Dip Switch init OK!\n");
	board->uart0->WriteString(board->uart0,
				"Dip Switch value = ");
	itoa(board->dipswitch->Read(board->dipswitch),
			str,2);
	board->uart0->WriteString(board->uart0,str);
	board->uart0->WriteString(board->uart0,"\n");
}

/*============================================================
 * Function: Delay()
 * Description: Just a simple delay, for application that
 * time precision is not necessary.
 * Paras:
 * 		>> 
 * Return:
 * 		>>
 * Change Record: 
 *		>> (08/Fev/2018): Creation of the function
 *============================================================*/
static void Delay()
{
	volatile int i = 0;

	for(i = 0; i < 0x7ffff; i++);
}

/*============================================================
 * Function: itoa()
 * Description: a standard itoa function
 * Paras:
 * 		>>
 * Return:
 * 		>>
 * Change Record:
 *		>> (08/Fev/2018): Creation of the function
 *============================================================*/
static void itoa(int value, char *str, int base)
{
	int i, cnt = 0, dividor;
	char res[32], heading_zero = 1;
	for(i = 0; i < 32; i++) res[i] = 0;

	i = 31;
	for(dividor = base; value != 0;){
		res[i--] = value%dividor;
		value = value/dividor;
	}

	for(i = 0; i < 32; i++)
	{
		if(heading_zero == 1 && res[i] == 0) continue;
		heading_zero = 0;
		str[cnt++] = res[i]+0x30;
	}
	str[cnt]='\0';
}
