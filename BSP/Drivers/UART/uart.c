/*
 * uart.c
 *
 *  Created on: 12 de dec de 2017
 *  Author: dong.x
 *  Version: 0.1
 *
 *  Description: Source file for uart module. Contains functions for
 *  CPU read/write, DMA r/w is not supported. The Autoflow control is not
 *  supported because in hardware, no autoflow control pin is connected.
 *
 * 	Change Records:
 *      >> (12/Dec/2017): Creation of function: UartInit();
 *      >> (13/Dec/2017): Add function:
 *                          UartReadChar;UartWriteChar;
 *                          UartReadCharBlock;UartWriteCharBlock
 *                        Complete function:
 *                          UartInit;
 *      >> (14/Dec/2017): Add function:
 *                          UartHandleInit;
 *      >> (16/Dec/2017): Create internal structure uart_internal_paras
 */

/*=========================================
 * Includes
 *=========================================*/
#include "../BSP/type.h"
#include "../BSP/conf.h"
#include "uart.h"
#include "../BSP/connector.h"
#include "string.h"
#include "hw_types.h"
#include "include/hw/soc_OMAPL138.h"
#include "hw_syscfg0_OMAPL138.h"
#include "include/uart.h"
#include "include/psc.h"
#include "include/armv5/omapl138/interrupt.h"

/*=========================================
 * Internal Variables
 *=========================================*/
    //11 bytes will be organized as follow:
    // byte[1-4]: uart register address
    // byte[5-8]: uart input frequency
    // byte[9-12]: uart config value
    // byte[13]: oversampling rate
    // byte[14]: fifo level
    // byte[15]: interrupt enable setting
    // byte[16-19]: baud rate
struct uart_internal_paras{
    uint32_t uart_addr;
    uint32_t uart_in_freq;
    uint32_t uart_conf;
    uint8_t uart_oversampling_rate;
    uint8_t uart_fifo_level;
    uint8_t uart_en_interrupt;
    uint32_t uart_baud_rate;
};

typedef struct uart_internal_paras uart_internal_t;


/*=========================================
 * Internal function declaration
 *=========================================*/
void DrvUartHandleInit(uart_handle_t *, uart_conf_t *);
/*=========================================
 * Functions
 *=========================================*/

/*============================================================
 * Function: DrvUartInit()
 * Description: Initilization of uart module.
 * 1. Initialize Uart module.
 * Para:
 *     >> uart_handle_t *: uart handle.
 *     >> uart_conf_t * uart configuration
 * Return:
 *     >>
 * Change Record:
 *		>> (12/Dec/2017): Creation of the function;
 *      >> (13/Dec/2017): Completion of the function;
 *      >> (16/Dec/2017): modification, add internal structure
 *
 *============================================================*/
void DrvUartInit(uart_handle_t * handle, uart_conf_t *ptr_cfg)
{
    uart_internal_t * ptr;
    unsigned int savePinmux = 0;

	PSCModuleControl(SOC_PSC_1_REGS,HW_PSC_UART1,PSC_POWERDOMAIN_ALWAYS_ON,PSC_MDCTL_NEXT_ENABLE);
    //Set GPIO setting
    savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(4)) &
    					  ~(0x22000000));
    	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(4)) =
    			 (0x22000000|savePinmux);

    // Initialize uart handle
    DrvUartHandleInit(handle,ptr_cfg);
    // Get uart data used for internal driver
    ptr = (uart_internal_t *)handle->ptr_uart_data;

    //  Reset transmitter and receiver
    UARTEnable(ptr->uart_addr);
    //  Set the divsor value DLH and DLL.
    UARTConfigSetExpClk(ptr->uart_addr,
       ptr->uart_in_freq,
       ptr->uart_baud_rate,
       ptr->uart_conf,
       ptr->uart_oversampling_rate);
    //  Set FIFOs setting: FCR
    UARTFIFOEnable(ptr->uart_addr);
    UARTFIFOLevelSet(ptr->uart_addr, ptr->uart_fifo_level);

    //  Enable uart interrupt
    UARTIntEnable(ptr->uart_addr, ptr->uart_en_interrupt);


}

/*============================================================
 * Function: DrvUartReadChar(uart_handle_t *)
 * Description: Uart Read Character without blocking
 * use UARTCharGetNonBlocking function.
  * Paras:
 *      >> uart_handle_t: uart handle
 * Return:
 *      >> uint32_t: -1 for read error
 *                   any other val for character read.
 * Change Record:
 *		>> (13/Dec/2017): Creation of the function
 *      >> (14/Dec/2017): Use handle instead of conf structure
 *                        as parameters for function.
 *============================================================*/
int32_t DrvUartReadChar(uart_handle_t * handle)
{
    return UARTCharGetNonBlocking(
        ((uart_internal_t *)handle->ptr_uart_data)->uart_addr);
}


/*============================================================
 * Function: DrvUartReadCharBlock(uart_handle_t *)
 * Description: Uart Read Character with blocking
 * use UARTCharGet function.
 * Paras:
 *      >> uart_handle_t: uart handle
 * Return:
 *      >> uint8_t: any other val for character read.
 * Change Record:
 *		>> (13/Dec/2017): Creation of the function
 *      >> (14/Dec/2017): Use handle instead of conf structure
 *                        as parameters for function.
 *============================================================*/
uint8_t DrvUartReadCharBlock(uart_handle_t * handle)
{
    return (uint8_t) UARTCharGet(
        ((uart_internal_t *)handle->ptr_uart_data)->uart_addr);
}

/*============================================================
 * Function: DrvUartWriteChar(uart_handle_t *, uint8)
 * Description: Uart Write Character with blocking
 * use UartCharPut function.
 * Paras:
 *      >> uart_handle_t: uart handle
 *      >> uint8_t: value to be written
 * Return:
 *      >>
 * Change Record:
 *		>> (13/Dec/2017): Creation of the function
 *      >> (14/Dec/2017): Use handle instead of conf structure
 *                        as parameters for function.
 *============================================================*/
void DrvUartWriteCharBlock(uart_handle_t * handle, uint8_t val)
{
    UARTCharPut(
        ((uart_internal_t *)handle->ptr_uart_data)->uart_addr, val);
}

/*============================================================
 * Function: DrvUartWriteStr(uart_handle_t *, String str)
 * Description: Uart Write Character with blocking
 * use UartCharPut function.
 * Paras:
 *      >> uart_handle_t: uart handle 
 *      >> uint8_t: value to be written
 * Return:
 *      >> 
 * Change Record: 
 *		>> (08/Fev/2018): Creation of the function
 *============================================================*/
void DrvUartWriteStr(uart_handle_t * handle, char * str)
{
    int cnt;

    for(cnt = 0; cnt < strlen(str); cnt++)
    {
        UARTCharPut(
        ((uart_internal_t *)handle->ptr_uart_data)->uart_addr, str[cnt]);
    }
    //UARTCharPut(
    //    ((uart_internal_t *)handle->ptr_uart_data)->uart_addr, '\n');
}

/*============================================================
 * Function: DrvUartWriteCharBlock(uart_handle_t *, uint8_t )
 * Description: Uart Write Character with blocking
 * use UartWriteCharBlock function.
 * Paras:
 *      >> uart_handle_t: uart handle
 *      >> uint8_t: value to be written
 * Return:
 *      >> bool_t: true for write sucessfully
 *                 false for error during writing.
 * Change Record:
 *		>> (13/Dec/2017): Creation of the function
 *      >> (14/Dec/2017): Use handle instead of conf structure
 *                        as parameters for function.
 *============================================================*/
bool_t DrvUartWriteChar(uart_handle_t * handle, uint8_t val)
{
    uint32_t return_val = 0;
    return_val = UARTCharPutNonBlocking(
        ((uart_internal_t *)handle->ptr_uart_data)->uart_addr, val);
    if(return_val == FALSE) return false;
    return true;
}

/*============================================================
 * Function: DrvUartHasChar(uart_handle_t *)
 * Description: This function checks if:
 *      a> the Receiver Buffer Register(RBR) has data and it is
 *         ready to be read (for non-FIFO mode) or
 *      b> the Receiver FIFO has atleast one byte of data to be
 *         read (FIFO mode).
 * Paras:
 *      >> uart_handle_t: uart handle
 * Return:
 *      >> bool_t: true for has at least one byte for read
 *                 false for has no data available.
 * Change Record:
 *		>> (13/Dec/2017): Creation of the function
 *      >> (14/Dec/2017): Use handle instead of conf structure
 *                        as parameters for function.
 *============================================================*/
bool_t DrvUartHasChar(uart_handle_t * handle)
{
    uint32_t return_val = 0;

    return_val = UARTCharsAvail (
        ((uart_internal_t *)handle->ptr_uart_data)->uart_addr);
    if(return_val == FALSE) return false;
    return true;
}

/*============================================================
 * Function: DrvUartIsTxRdy(uart_handle_t *)
 * Description: This function checks if:
 *      a> if the transmitter is empty by checking for the
 *         emptiness of Transmitter Holding Register(THR)
 *         and the Transmit Shift regsiter(TSR).
 * Paras:
 *      >> uart_handle_t: uart handle
 * Return:
 *      >> bool_t: true for tx is avaliable
 *                 false for tx not available.
 * Change Record:
 *		>> (13/Dec/2017): Creation of the function
 *      >> (14/Dec/2017): Use handle instead of conf structure
 *                        as parameters for function.
 *============================================================*/
bool_t DrvUartIsTxRdy(uart_handle_t * handle)
{
    uint32_t return_val = 0;

    return_val = UARTSpaceAvail(
        ((uart_internal_t *)handle->ptr_uart_data)->uart_addr);
    if(return_val == FALSE) return false;
    return true;
}


/*============================================================
 * Function: DrvUartDisable(uart_handle_t *)
 * Description: Disable the uart and release the handle memory
 * Paras:
 *      >> uart_handle_t: uart handle
 * Return:
 *      >>
 * Change Record:
 *		>> (14/Dec/2017): Creation of the function
 *============================================================*/
void DrvUartDisable(uart_handle_t * handle)
{
    //free the memory of handle and Disable the uart
    UARTDisable(((uart_internal_t *)handle->ptr_uart_data)->uart_addr);

    free(handle->ptr_uart_data);
}

/*============================================================
 * Function: DrvHandleInit(uart_handle_t *)
 * Description: Initilize handle struture according to the cfg
 * Paras:
 *      >> uart_handle_t *: uart handle
 *      >> uart_conf_t *: uart configuration
 * Return:
 *      >>
 * Change Record:
 *		>> (14/Dec/2017): Creation of the function
 *      >> (16/Dec/2017): Simplifies the fucntion with internal
 *          data structure.
 *============================================================*/
void DrvUartHandleInit(uart_handle_t * handle, uart_conf_t *ptr_conf)
{
    uart_internal_t * uart_paras;

    // Create internal use data area
    handle->ptr_uart_data = malloc(sizeof(uart_internal_t));
    uart_paras = (uart_internal_t *) handle->ptr_uart_data;


    //Set uart register address and input frequency
    if(ptr_conf->uart_id == 0){
        uart_paras->uart_addr = SOC_UART_0_REGS;
        uart_paras->uart_in_freq = UART_0_FREQ;
    }
    else if(ptr_conf->uart_id == 1){
        uart_paras->uart_addr = SOC_UART_1_REGS;
        uart_paras->uart_in_freq = UART_1_2_FREQ;
    }
    else if(ptr_conf->uart_id == 2){
        uart_paras->uart_addr = SOC_UART_2_REGS;
        uart_paras->uart_in_freq = UART_1_2_FREQ;
    }

    //Set data length of uart
    switch(ptr_conf->data_len)
    {
        case 0: uart_paras->uart_conf = UART_WORDL_8BITS;
                break;
        case 1: uart_paras->uart_conf = UART_WORDL_5BITS;
                break;
        case 2: uart_paras->uart_conf = UART_WORDL_6BITS;
                break;
        case 3: uart_paras->uart_conf = UART_WORDL_7BITS;
                break;
        default: break;
    }

    //Set priority and stop bits
    switch(ptr_conf->priority_stop_bits)
    {
        case 2: uart_paras->uart_conf |= UART_STOP_BIT;
                break;
        case 5: uart_paras->uart_conf |= UART_PARITY;
                break;
        case 6: uart_paras->uart_conf |= (UART_PARITY|UART_STOP_BIT);
                break;
        case 9: uart_paras->uart_conf |= (UART_SET_PARITY_TYPE|
                UART_PARITY);break;
        case 10: uart_paras->uart_conf |= (UART_SET_PARITY_TYPE|
                UART_PARITY|UART_STOP_BIT);break;
        default: break;
    }

    //Set oversampling configuration
    if(ptr_conf->over_sample_rate == 0){
        uart_paras->uart_oversampling_rate = UART_OVER_SAMP_RATE_16;
    }
    else{
        uart_paras->uart_oversampling_rate = UART_OVER_SAMP_RATE_13;
    }

    // Set fifo level
    switch(ptr_conf->fifo_level)
    {
        case 1: uart_paras->uart_fifo_level = UART_RX_TRIG_LEVEL_1;break;
        case 2: uart_paras->uart_fifo_level = UART_RX_TRIG_LEVEL_4;break;
        case 3: uart_paras->uart_fifo_level = UART_RX_TRIG_LEVEL_8;break;
        case 4: uart_paras->uart_fifo_level = UART_RX_TRIG_LEVEL_14;break;
        default: uart_paras->uart_fifo_level = 0; break;
    }

    //Set interrupt value
    if(ptr_conf->interrupt_en_bits & 0x01)
    {
        uart_paras->uart_en_interrupt |= UART_INT_MODEM_STAT;
    }
    if(ptr_conf->interrupt_en_bits & 0x02)
    {
        uart_paras->uart_en_interrupt |= UART_INT_LINE_STAT;
    }
    if(ptr_conf->interrupt_en_bits & 0x04)
    {
        uart_paras->uart_en_interrupt |= UART_INT_TX_EMPTY;
    }
    if(ptr_conf->interrupt_en_bits & 0x08)
    {
        uart_paras->uart_en_interrupt |= UART_INT_RXDATA_CTI;
    }

    uart_paras->uart_baud_rate = ptr_conf->baud_rate;
}

