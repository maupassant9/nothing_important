/*
 * spi.h
 *
 *  Created on: 22 de Dec de 2017
 *  Author: dong.x
 *  Change Records:
 *     >> (26/Dec/2017): File Created.
 */

#ifndef BSP_DRIVERS_COMMON_SPI_SPI_H_
#define BSP_DRIVERS_COMMON_SPI_SPI_H_

#include "../BSP/type.h"
#include "../BSP/connector.h"

/*============================================================
 * Macro
 *============================================================*/


/*============================================================
 *Typedef
 *============================================================*/



/*============================================================
 *Function Prototype
 *============================================================*/

/*============================================================
 * Function: DrvSpiInit()
 * Para:
 *    >> spi_handle_t * handle: a spi handle
 *    >> spi_conf_t * ptr: spi configuration structure
 * Return:
 *     >>
 *  Usage: Initilization of spi module. The first function
 * should be called before any other spi function.
 * Change Record:
 *		>> (22/Dec/2017): Creation of the function;
 *
 *============================================================*/
void DrvSpiInit(spi_handle_t * handle, spi_conf_t * ptr_conf);

/*============================================================
 * Function: DrvWrite()
 * Description: Write to specific slave
 * Para:
 *    >> spi_conf_t * ptr: spi configuration structure
 * Return:
 *     >> uint8_t: -1: error occured;
 *                     any value > 0 success.
 * Change Record:
 *		>> (27/Dec/2017): Creation and realization;
 *
 *============================================================*/
void DrvSpiWrite(spi_handle_t * handle, uint32_t data);


/*============================================================
 * Function: DrvWriteDma()
 * Description: Write to specific slave using DMA
 * Para:
 *    >> spi_conf_t * ptr: spi configuration structure
 * Return:
 *     >> uint8_t: -1: error occured;
 *                     any value > 0 success.
 * Change Record:
 *		>> (27/Dec/2017): Creation and realization;
 *
 *============================================================*/
uint8_t DrvSpiWriteDma(spi_handle_t * handle);


/*============================================================
 * Function: DrvRead()
 * Description: Read from specific slave
 * Para:
 *    >> spi_conf_t * ptr: spi configuration structure
 * Return:
 *     >> uint8_t: -1: error occured;
 *                     any value > 0 success.
 * Change Record:
 *		>> (27/Dec/2017): Creation and realization;
 *
 *============================================================*/
uint32_t DrvSpiRead(spi_handle_t * handle);

/*============================================================
 * Function: DrvReadDma()
 * Description: Read from specific slave using DMA
 * Before calling this function, function DrvSpiAttachBuffer
 * should be called.
 * Para:
 *    >> spi_conf_t * ptr: spi configuration structure
 * Return:
 *     >> uint8_t: -1: error occured;
 *                     any value > 0 success.
 * Change Record:
 *		>> (26/Dec/2017): Creation and realization;
 *
 *============================================================*/
uint8_t DrvSpiReadDma(spi_handle_t * handle);

/*============================================================
 * Function: DrvAttachBuffer()
 * Description: Read from specific slave using DMA
 * Para:
 *    >> spi_handle_t * ptr: spi handle
 *    >> bool_t: true for write buffer, false for read buffer
 *    >> void *: a pointer to a buffer
 *    >> uint32_t: the size of buffer (in bytes)
 * Return:
 *     >>
 * Change Record:
 *		>> (26/Dec/2017): Creation and realization;
 *
 *============================================================*/
void DrvSpiAttachBuffer(spi_handle_t * handle, bool_t is_write,
                        void * ptr_buffer,uint32_t buffer_sz);

/*============================================================
 * Function: DrvSpiSetCs()
 * Description: Write to specific slave, this function only write
 * to TX part of SPIDAT1 register.
 * Usage:
 *    >> spi_conf_t * ptr: spi configuration structure
 *    >> uint8_t: the corresponding cs pin that need to use next time
 *                cs = 0, cs_0; cs = 1, cs_1; cs = 2, cs_2;
 *                cs = 3, cs_3, ....
 * Return:
 *     >>
 * Change Record:
 *		>> (04/Jan/2018): Creation and realization;
 *
 *============================================================*/
void DrvSpiSetCs(spi_handle_t * handle, uint8_t cs);


/*============================================================
 * Function: SpiCsHold()
 * Description: CS hold Configuration.
 * Para:
 *    >> spi_conf_t * ptr: spi configuration structure
 *    >> bool_t: set (1) or clear CS hold bit.
 * Return:
 *     >>
 * Change Record:
 *		>> (08/Jan/2018): Creation and realization;
 *
 *============================================================*/
void DrvSpiCsHold(spi_handle_t * handle, bool_t cs_hold);


#endif /* BSP_DRIVERS_COMMON_SPI_SPI_H_ */
