/*
 * tdc.c
 *
 *  Created on: 03 de Jan de 2018
 *  Author: dong.x
 *  Version: 0.1
 *
 *  Description: Source file for tdc7201.
 * 	Change Records:
 *      >> (3/Jan/2017): File Created.
 *                        Create function: DrvTdcInit
 *      >> (): DrvTdcRead, DrvTdcReadDma,
 *                        DrvTdcWrite, DrvTdcWriteDma
 */

/*=========================================
 * Includes
 *=========================================*/
#include "../BSP/type.h"
#include "../BSP/conf.h"
#include "../BSP/connector.h"
#include "../BSP/board.h"
#include <stdlib.h>

#include "hw_types.h"
#include "include/hw/soc_OMAPL138.h"
#include "hw_syscfg0_OMAPL138.h"
#include "include/armv5/omapl138/interrupt.h"
#include "../COMMON/SPI/spi.h"
#include "include/psc.h"
#include "include/gpio.h"
#include "tdc.h"
#include "include/edma.h"
#include "include/spi.h"

//ONE_READ_LENGTH should be a number of factor 4
#define ONE_READ_LENGTH 400
/*=========================================
 * Internal Variables
 *=========================================*/
//Internal structure for dma
typedef  struct{
    //These two buffer are used
    // to help DMA control.
	//Read buffer and Write buffer.
    uint32_t wt_buffer[20];
    uint16_t rd_buffer[21*ONE_READ_LENGTH];
    //commands used to complete DMA transfer
    uint32_t cmds[19];
    uint32_t spi_data1_val;
    //data used for dma wasting time.
    uint16_t waste;
    //Param set for CHANNEL_WR
    EDMA3CCPaRAMEntry param_set_wr;
    //Param set for CHANNEL_RD
    EDMA3CCPaRAMEntry param_set_rd;
}dma_internal_t;

//a channel structure for tdc
typedef struct tdc_channel_t{
	uint8_t ch_no; // channel number
	uint16_t set_cs_cmd; //cs value for every channel
	struct tdc_channel_t * next;
}tdc_channel_t;

//Internal data strucutre.
typedef  struct {
    uint8_t dev_id;
    uint8_t cfg1; //used to save the cfg1 reg number
    uint8_t cfg2;
    uint8_t cali_period;

    tdc_channel_t * ch1;
    tdc_channel_t * ch2;
    tdc_channel_t * ch3;
    tdc_channel_t * ch0;
    tdc_channel_t * curr;

    spi_handle_t * spi_handle;
    //A dma structure
    dma_internal_t dma;
} tdc_internal_t;




/*=========================================
 * Internal function declaration
 *=========================================*/
static void TdcHandleInit(tdc_handle_t * handle, tdc_conf_t * ptr_conf);
static void TdcPscGpioInit(tdc_handle_t * handle);
static void TdcCalc(tdc_handle_t * handle, int index, uint32_t *result);
static uint8_t TdcReadReg(tdc_handle_t * handle, uint16_t addr);
static void TdcReadRegSeq(tdc_handle_t * handle, uint16_t addr, uint8_t num);
static void TdcCalc(tdc_handle_t * handle, int index, uint32_t *result);
static void TdcReadRegSeqDma(tdc_handle_t * handle);
static void TdcWriteReg(tdc_handle_t * handle, uint16_t addr, uint8_t val);
static void TdcDmaInit(tdc_handle_t * handle);
static void TdcEnterAutoMode(tdc_handle_t * handle);


/*=========================================
 * Functions
 *=========================================*/

/*============================================================
 * Function: DrvTdcInit()
 * Description: Initilization of TDC7201 according to the
 * configuration structure.
 * Para:
 *    >> tdc_handle_t * handle: a tdc7201 handle
 *    >> tdc_conf_t * ptr: tdc configuration structure
 * Return:
 *     >>
 * Change Record:
 *		>> (03/Jan/2018): Creation of the function;
 *      >> (/Jan/2018): Realization.
 *
 *============================================================*/
void DrvTdcInit(tdc_handle_t * handle,
                void * ptr_conf)
{

    //Config the psc and GPIO
    TdcPscGpioInit(handle);
	//Initialize the tdc_handle_t structure
    TdcHandleInit(handle,(tdc_conf_t *)ptr_conf);
}


/*============================================================
 * Function: DrvTdcHandleInit()
 * Description: Initilization of TDC7201 according to the
 * configuration structure.
 * Para:
 *    >> tdc_handle_t * handle: a tdc7201 handle
 *    >> tdc_conf_t * ptr: tdc configuration structure
 * Return:
 *     >>
 * Change Record:
 *		>> (03/Jan/2018): Creation of the function;
 *      >> (03/Jan/2018): Realization.
 *
 *============================================================*/
static void TdcHandleInit(tdc_handle_t * handle, tdc_conf_t * ptr_conf)
{
    tdc_internal_t * ptr_internal;
    void * comm = handle->ptr_internal;
    EDMA3CCPaRAMEntry * ptr_param_set;

    handle->ptr_internal = malloc(sizeof(tdc_internal_t));
    ptr_internal = (tdc_internal_t *)(handle->ptr_internal);

	if(comm == NULL)
	{
		ptr_internal->spi_handle = ptr_conf->ptr_spi_handle;
	}
	else
	{
		ptr_internal->spi_handle = (spi_handle_t *)comm;
	}
    //handle->dev_id = ptr_conf->dev_id;

	//create channel struct
	ptr_internal->ch1 = (tdc_channel_t *)malloc(sizeof(tdc_channel_t));
	ptr_internal->ch2 = (tdc_channel_t *)malloc(sizeof(tdc_channel_t));
	ptr_internal->ch3 = (tdc_channel_t *)malloc(sizeof(tdc_channel_t));
	ptr_internal->ch0 = (tdc_channel_t *)malloc(sizeof(tdc_channel_t));

	ptr_internal->ch0->ch_no = 0;
	ptr_internal->ch1->ch_no = 1;
	ptr_internal->ch2->ch_no = 2;
	ptr_internal->ch3->ch_no = 3;

	ptr_internal->ch0->set_cs_cmd = TDC1_CS1_NUM;
	ptr_internal->ch1->set_cs_cmd = TDC1_CS2_NUM;
	ptr_internal->ch2->set_cs_cmd = TDC2_CS1_NUM;
	ptr_internal->ch3->set_cs_cmd = TDC2_CS2_NUM;

	ptr_internal->ch0->next = ptr_internal->ch1;
	ptr_internal->ch1->next = ptr_internal->ch2;
	ptr_internal->ch2->next = ptr_internal->ch3;
	ptr_internal->ch3->next = ptr_internal->ch0;

	ptr_internal->curr = ptr_internal->ch0;

//    switch(handle->dev_id)
//    {
//        case 0: ptr_internal->set_cs_cmd = TDC1_CS1_NUM;
//            break;
//        case 1: ptr_internal->set_cs_cmd = TDC1_CS2_NUM;
//            break;
//        case 2: ptr_internal->set_cs_cmd = TDC2_CS1_NUM;
//            break;
//        case 3: ptr_internal->set_cs_cmd = TDC2_CS2_NUM;
//            break;
//        default: break;
//    }

    //Configuration TDC
    ptr_internal->cfg1 = TdcReadReg(handle, TDC_CFG1);
    ptr_internal->cfg2 = TdcReadReg(handle, TDC_CFG2);

    if(ptr_conf->is_rising_edge == true)
    {
        ptr_internal->cfg1 &= 0xe7;
    }
    else{
        ptr_internal->cfg1 |= 0x18;
    }
    //set measurement mode as 2
    ptr_internal->cfg1 &= 0xfb;
    ptr_internal->cfg1 |= 0x02;

    //Calibration period setting
    //TODO: here set to 00, need to check a better
    // value in the future.
    ptr_internal->cfg2 &= 0x3f;
    ptr_internal->cali_period = 2;
    //Multiple cycle aging setting
    ptr_internal->cfg2 &= 0xc7;
    switch(ptr_conf->multi_cycle_aging)
    {
        case 1: break;
        case 2: ptr_internal->cfg2 |= 0x08;
                break;
        case 4: ptr_internal->cfg2 |= 0x10;
                break;
        case 8: ptr_internal->cfg2 |= 0x18;
                break;
        case 16: ptr_internal->cfg2 |= 0x20;
                break;
        case 32: ptr_internal->cfg2 |= 0x28;
                break;
        case 64: ptr_internal->cfg2 |= 0x30;
                break;
        case 128: ptr_internal->cfg2 |= 0x38;
                break;
        default: break;
    }
    //number of stop setting.
    ptr_internal->cfg2 &= 0xf8;
    ptr_internal->cfg2 |= (ptr_conf->stop_number - 1);
    //write to tdc register: ch0
    TdcWriteReg(handle, TDC_CFG1, ptr_internal->cfg1);
    TdcWriteReg(handle, TDC_CFG2, ptr_internal->cfg2);
    DrvTdcNextChannel(handle);
    TdcWriteReg(handle, TDC_CFG1, ptr_internal->cfg1);
    TdcWriteReg(handle, TDC_CFG2, ptr_internal->cfg2);
    DrvTdcNextChannel(handle);
    TdcWriteReg(handle, TDC_CFG1, ptr_internal->cfg1);
    TdcWriteReg(handle, TDC_CFG2, ptr_internal->cfg2);
    DrvTdcNextChannel(handle);
    TdcWriteReg(handle, TDC_CFG1, ptr_internal->cfg1);
    TdcWriteReg(handle, TDC_CFG2, ptr_internal->cfg2);
    DrvTdcNextChannel(handle);

    //Here init the DMA for read which will be used a
    // lot in the future
    ptr_param_set = &ptr_internal->dma.param_set_rd;
    ptr_param_set->srcAddr = ptr_internal->spi_handle->base_addr + 0x40; //address of SPIBUF
    ptr_param_set->destAddr = (uint32_t)&ptr_internal->dma.rd_buffer[0]; //write buffer+1 (16bit)
    ptr_param_set->bCnt = 20;
    ptr_param_set->srcBIdx = 0;
    ptr_param_set->destBIdx = 2;
    ptr_param_set->srcCIdx = 0;
    ptr_param_set->destCIdx =  0;
    ptr_param_set->bCntReload = 20; //no reload needed
	ptr_param_set->opt = 0x00101100;
	ptr_param_set->linkAddr = 0xffff;
	ptr_param_set->aCnt = 2;
	ptr_param_set->cCnt = 1;

	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS,
						EDMA3_CHANNEL_TYPE_DMA,
						CHANNEL_NO_RD,  //channel number
						1,  //tcc number
						0); //event queue number
}

/*============================================================
 * Function: TdcPscGpioInit()
 * Description: Initilization of PSC module and corresponding GPIO
 * The only module used is spi module here, and before any Tdc handle
 * initilized, the spi module should already been initialized, so no
 * psc control needed.
 * Para:
 *    >> tdc_handle_t * handle: a tdc7201 handle
 * Return:
 *     >>
 * Change Record:
 *		>> (03/Jan/2018): Creation of the function;
 *      >> (05/Jan/2018): Realization.
 *
 *============================================================*/
static void TdcPscGpioInit(tdc_handle_t * handle)
{
    uint32_t save_pin_mux;

	 //Set the EN PINs
	// gpio mutex Setting
	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(TDC1_EN_PIN_MUX_NUM)) &
				  ~TDC1_EN_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(TDC1_EN_PIN_MUX_NUM)) =
				 (TDC1_EN_PIN_MUX_VAL | save_pin_mux);
	//set to output
	GPIODirModeSet(SOC_GPIO_0_REGS, TDC1_EN_PIN, GPIO_DIR_OUTPUT);
	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(TDC2_EN_PIN_MUX_NUM)) &
				  ~TDC2_EN_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(TDC2_EN_PIN_MUX_NUM)) =
				 (TDC2_EN_PIN_MUX_VAL | save_pin_mux);
	GPIODirModeSet(SOC_GPIO_0_REGS, TDC2_EN_PIN, GPIO_DIR_OUTPUT);

	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(TDC1_INT1_PIN_MUX_NUM)) &
			  ~TDC1_INT1_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(TDC1_INT1_PIN_MUX_NUM)) =
			 (TDC1_INT1_PIN_MUX_VAL | save_pin_mux);
	GPIODirModeSet(SOC_GPIO_0_REGS, TDC1_INT1_PIN, GPIO_DIR_INPUT);
	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(TDC1_INT2_PIN_MUX_NUM)) &
			  ~TDC1_INT2_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(TDC1_INT2_PIN_MUX_NUM)) =
			 (TDC1_INT2_PIN_MUX_VAL | save_pin_mux);
	GPIODirModeSet(SOC_GPIO_0_REGS, TDC1_INT2_PIN, GPIO_DIR_INPUT);
	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(TDC2_INT1_PIN_MUX_NUM)) &
			  ~TDC2_INT1_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(TDC2_INT1_PIN_MUX_NUM)) =
			 (TDC2_INT1_PIN_MUX_VAL | save_pin_mux);
	GPIODirModeSet(SOC_GPIO_0_REGS, TDC2_INT1_PIN, GPIO_DIR_INPUT);
	save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(TDC2_INT2_PIN_MUX_NUM)) &
			  ~TDC2_INT2_PIN_MUX_LOC);
	HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(TDC2_INT2_PIN_MUX_NUM)) =
			 (TDC2_INT2_PIN_MUX_VAL | save_pin_mux);
	GPIODirModeSet(SOC_GPIO_0_REGS, TDC2_INT2_PIN, GPIO_DIR_INPUT);
    //Enable GPIO BANK 0 interrupt. TODO:
	GPIOIntTypeSet(SOC_GPIO_0_REGS, TDC1_INT1_PIN, GPIO_INT_TYPE_RISEDGE);
	GPIOIntTypeSet(SOC_GPIO_0_REGS, TDC1_INT2_PIN, GPIO_INT_TYPE_RISEDGE);
	GPIOIntTypeSet(SOC_GPIO_0_REGS, TDC2_INT1_PIN, GPIO_INT_TYPE_RISEDGE);
	GPIOIntTypeSet(SOC_GPIO_0_REGS, TDC2_INT2_PIN, GPIO_INT_TYPE_RISEDGE);

	GPIOBankIntDisable(SOC_GPIO_0_REGS, 0);
	DrvTdcEnable(handle);
}



/*============================================================
 * Function: TdcReadReg()
 * Description: Read the TDC7201 register
 * Para:
 *    >> tdc_handle_t * handle: a tdc7201 handle
 * Return:
 *     >>
 * Change Record:
 *		>> (04/Jan/2018): Creation of the function;
 *      >> (04/Jan/2018): Realization.
 *      >> (05/Jan/2018): tested ok.
 *
 *============================================================*/
static uint8_t TdcReadReg(tdc_handle_t * handle, uint16_t addr)
{
    tdc_internal_t * ptr_internal;

    ptr_internal = (tdc_internal_t *)(handle->ptr_internal);
    ptr_internal->spi_handle->SetCs(
                ptr_internal->spi_handle, ptr_internal->curr->set_cs_cmd);

    //first write to register
    ptr_internal->spi_handle->Write(ptr_internal->spi_handle, addr);
    //then read the content
    return(ptr_internal->spi_handle->Read(ptr_internal->spi_handle));
}

/*============================================================
 * Function: TdcReadRegSeq()
 * Description: Sequent Read the TDC7201 register
 * The results will be stored in ptr_internal->dma.rd_buffer.
 *    		data[0]: 0xffXX valid data XX
 *    		data[1]: 0xXXXX
 *    		data[2]: 0xXXXX
 *    		....
 * Para:
 *    >> tdc_handle_t * handle: a tdc7201 handle
 *    >> uint16_t: address of register of tdc7201
 *    >> uint16_t: number of read regs
 * Return:
 *     >>
 * Change Record:
 *		>> (04/Jan/2018): Creation of the function;
 *      >> (05/Jan/2018): Realization.
 *      >> (08/Jan/2018): tested ok.
 *
 *============================================================*/
static void TdcReadRegSeq(tdc_handle_t * handle, uint16_t addr, uint8_t num)
{
    tdc_internal_t * ptr_internal = (tdc_internal_t *)(handle->ptr_internal);
    uint32_t cnt = 0, cmd[4];
    uint32_t wr_reg_addr = (uint32_t)ptr_internal->spi_handle->base_addr;

    wr_reg_addr += 0x38;
    addr = addr << 8;
    cmd[0] = 0x10348000|addr;
    cmd[1] = 0x10388000|addr;
    cmd[2] = 0x101c8000|addr;
    cmd[3] = 0x102c8000|addr;


	//Init the para_set for read
	EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, CHANNEL_NO_RD, &ptr_internal->dma.param_set_rd);
	SPIIntEnable(ptr_internal->spi_handle->base_addr, SPI_DMA_REQUEST_ENA_INT);
	EDMA3EnableDmaEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_RD);

    HWREG(wr_reg_addr+4) = cmd[ptr_internal->curr->ch_no];
    for(cnt = 1;cnt < num; cnt++)
	{
    	HWREG(wr_reg_addr) = 0;
	}
    SPIIntDisable(ptr_internal->spi_handle->base_addr, SPI_DMA_REQUEST_ENA_INT);
    EDMA3DisableDmaEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_RD);

}

/*============================================================
 * Function: TdcReadRegSeqDma
 * Description: Sequence read using DMA the 13 result register
 * of TDC7201, this function uses DMA to decrease the maximum
 * possible the interference of CPU. DMA channel 14 and 15 is
 * used. CH14 used for data read from spi, and ch15 used for
 * data write to spi. The data from spi is saved in
 * ptr_internal->dma.rd_buffer.
 *
 * Para:
 *    >> tdc_handle_t : a tdc7201 handle
 * Return:
 *     >>
 * Change Record:
 *		>> (12/Jan/2018): Creation of the function;
 *
 *============================================================*/
static void TdcReadRegSeqDma(tdc_handle_t * handle)
{
    tdc_internal_t * ptr_internal = (tdc_internal_t *)handle->ptr_internal;
    spi_handle_t * spi_handle = ptr_internal->spi_handle;

    ptr_internal = (tdc_internal_t *)(handle->ptr_internal);

	//1 - Clear the Event and Event error
	EDMA3ClrMissEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_WR);
	EDMA3ClrEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_WR);
	EDMA3ClrMissEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_RD);
	EDMA3ClrEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_RD);

	//2 - Set up param
	EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, CHANNEL_NO_WR,
					&ptr_internal->dma.param_set_wr);
	EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, CHANNEL_NO_RD,
					&ptr_internal->dma.param_set_rd);

	//3 - Prepare for SPI
	spi_handle->CsHold(ptr_internal->spi_handle, true, ptr_internal->curr->set_cs_cmd);
	//4 - Enable event/Start DMA control
	EDMA3EnableDmaEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_RD);
	EDMA3EnableDmaEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_WR);
	//5 - Write one word to spi to begin to generate event
	spi_handle->Write(ptr_internal->spi_handle,ptr_internal->dma.wt_buffer[0]);
	//6 - Polling for completion: waiting for read complete
	while(!(HWREG(SOC_EDMA30CC_0_REGS+0x1068)&0x00000002));
	HWREG(SOC_EDMA30CC_0_REGS+0x1070) = 0x000000002;
	//7 - Stop the spi cs_hold mode and disable event
	spi_handle->CsHold(ptr_internal->spi_handle, false, ptr_internal->curr->set_cs_cmd);
	EDMA3DisableDmaEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_WR);
	EDMA3DisableDmaEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_RD);
}

/*============================================================
 * Function: TdcWriteReg()
 * Description: Write the TDC7201 register
 * Para:
 *    >> tdc_handle_t * handle: a tdc7201 handle
 * Return:
 *     >>
 * Change Record:
 *		>> (04/Jan/2018): Creation of the function;
 *      >> (04/Jan/2018): Realization.
 *      >> (05/Jan/2018): Tested OK.
 *
 *============================================================*/
static void TdcWriteReg(tdc_handle_t * handle, uint16_t addr, uint8_t val)
{
    TdcReadReg(handle,addr|val|IS_WRITE);
}

/*============================================================
 * Function: DrvTdcEnable()
 * Description: Enable the TDC720, Pull up the enable pin
 * Para:
 *    >> tdc_handle_t * handle: a tdc7201 handle
 * Return:
 *     >>
 * Change Record:
 *		>> (04/Jan/2018): Creation of the function;
 *      >> (05/Jan/2018): Realization.
 *
 *============================================================*/
void DrvTdcEnable(tdc_handle_t * handle)
{
        GPIOPinWrite(SOC_GPIO_0_REGS, TDC1_EN_PIN,0x01);
        GPIOPinWrite(SOC_GPIO_0_REGS, TDC2_EN_PIN,0x01);
}

/*============================================================
 * Function: DrvTdcDisable()
 * Description: Disable the TDC7201, pull down the enable pin
 * Para:
 *    >> tdc_handle_t * handle: a tdc7201 handle
 * Return:
 *     >>
 * Change Record:
 *		>> (04/Jan/2018): Creation of the function;
 *      >> (05/Jan/2018): Realization.
 *
 *============================================================*/
 void DrvTdcDisable(tdc_handle_t * handle)
{
    GPIOPinWrite(SOC_GPIO_0_REGS, TDC1_EN_PIN,0x00);
    GPIOPinWrite(SOC_GPIO_0_REGS, TDC2_EN_PIN,0x00);
}

/*============================================================
 * Function: DrvTdcGetResult()
 * Description: Get the result of TDC, without using DMA.
 * Para:
 *    >> tdc_handle_t * handle: a tdc7201 handle
 *    >> uint32_t: result of TOF.
 * Return:
 *     >> uint32_t: result of Tdc in ns.
 * Change Record:
 *		>> (05/Jan/2018): Creation of the function;
 *      >> (05/Jan/2018): Realization.
 *      >> (29/Jan/2018): Seperate the calculate func
 *      				  and the TDC read sequence func.
 *
 *============================================================*/
void DrvTdcGetResult(tdc_handle_t * handle, uint32_t *result)
{
    TdcReadRegSeq(handle,0x10,20);
    //TdcReadRegSeqDma(handle);
    //Analise the values and find out the TOF results.
    TdcCalc(handle, 0, result);
}


/*============================================================
 * Function: DrvTdcStart()
 * Description: Start the measurement. This should be used:
 * (1) in non-DMA mode: to start a measure.
 * (2) in DMA mode: before start DMA, used to initialize the
 * 					transfer.
 * Para:
 *    >> tdc_handle_t * handle: a tdc7201 handle
 * Return:
 *     >>
 * Change Record:
 *		>> (05/Jan/2018): Creation of the function;
 *      >> (05/Jan/2018): Realization.
 *
 *============================================================*/
void DrvTdcStart(tdc_handle_t * handle)
{
	//TODO: Test
    tdc_internal_t * ptr_internal;
    ptr_internal = (tdc_internal_t *) handle->ptr_internal;
    //write to corrsponding register
    ptr_internal->cfg1 |= 0b00000001;
    TdcWriteReg(handle, TDC_CFG1, ptr_internal->cfg1);
}


/*============================================================
 * Function: TdcDmaInit
 * Description: DMA perpherial initialization for TDC7201.
 * This function is used only inside Tdc driver, should not be
 * called outside the tdc driver scope. The function initialize
 * the DMA controller for sequence read of all the result
 * registers.
 * Para:
 *    >> tdc_handle_t: tdc handle
 * Return:
 *    >>
 * Change Record:
 *    >> (12/Jan/2018): Creation of the function;
 *
 *============================================================*/
 static void TdcDmaInit(tdc_handle_t * handle)
 {
	tdc_internal_t * ptr_internal = (tdc_internal_t *)handle->ptr_internal;
	spi_handle_t * spi_handle = ptr_internal->spi_handle;
	EDMA3CCPaRAMEntry *ptr_param_set, param_set;
	uint16_t cnt = 0;

    //Initiate the buffer for DMA.
    for(;cnt < 20; cnt++) {
    	ptr_internal->dma.wt_buffer[cnt] = 0;
    	ptr_internal->dma.rd_buffer[cnt] = 0;
    }
    ptr_internal->dma.wt_buffer[0] = 0x1000|0x8000;
    //ptr_internal->dma.spi_data1_val = 0x0010&(0x01<<cs);


	//1 - Init the param_set
	//Init the param_set for write
    ptr_param_set = &ptr_internal->dma.param_set_wr;
    ptr_param_set->srcAddr = (uint32_t)&ptr_internal->dma.wt_buffer[1]; //write buffer+1 (16bit)
    ptr_param_set->destAddr = spi_handle->base_addr + 0x38; //address of SPIDAT0
    ptr_param_set->bCnt = 19;
    ptr_param_set->srcBIdx = 2;
    ptr_param_set->destBIdx = 0;
    ptr_param_set->srcCIdx = 0;
    ptr_param_set->destCIdx =  0;
    ptr_param_set->bCntReload = 19; //no reload needed
    ptr_param_set->opt = 0x00100100;
    ptr_param_set->linkAddr = 0xffff;
    ptr_param_set->aCnt = 2; //16 bits
    ptr_param_set->cCnt = 1;
    EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, CHANNEL_NO_WR, ptr_param_set);

	//Init the para_set for read
    ptr_param_set = &ptr_internal->dma.param_set_rd;
    ptr_param_set->srcAddr = spi_handle->base_addr + 0x40; //address of SPIBUF
    ptr_param_set->destAddr = (uint32_t)&ptr_internal->dma.rd_buffer[0]; //write buffer+1 (16bit)
    ptr_param_set->bCnt = 20;
    ptr_param_set->srcBIdx = 0;
    ptr_param_set->destBIdx = 2;
    ptr_param_set->srcCIdx = 0;
    ptr_param_set->destCIdx =  0;
    ptr_param_set->bCntReload = 20; //no reload needed
	ptr_param_set->opt = 0x00101100;
	ptr_param_set->linkAddr = 0xffff;
	ptr_param_set->aCnt = 2;
	ptr_param_set->cCnt = 1;
	EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, CHANNEL_NO_RD, ptr_param_set);

	//Init the param_set for gpio bank 0 (for INT1..4)
	ptr_param_set = &param_set;
	ptr_param_set->srcAddr = (uint32_t)&ptr_internal->dma.wt_buffer[0]; //write buffer+1 (16bit)
	ptr_param_set->destAddr = spi_handle->base_addr + 0x3c; //address of SPIDAT1, for cfg
	ptr_param_set->bCnt = 1;
	ptr_param_set->srcBIdx = 0;
	ptr_param_set->destBIdx = 0;
	ptr_param_set->srcCIdx = 0;
	ptr_param_set->destCIdx =  0;
	ptr_param_set->bCntReload = 1; //no reload needed
	ptr_param_set->opt = 0x00101100;
	ptr_param_set->linkAddr = 0xffff;
	ptr_param_set->aCnt = 2;
	ptr_param_set->cCnt = 1;
	EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, CHANNEL_NO_INIT_0, ptr_param_set);

	//2 - Clear the Event
	EDMA3ClrMissEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_WR);
	EDMA3ClrMissEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_RD);

	//3 - Init DMA
	EDMA3Init(SOC_EDMA30CC_0_REGS, 0);
	//4 - DMA channel and TCC.
	//    DMA0 channel 15/19 is used; tcc not used
	//    event Q0 is arragend.
	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS,
						EDMA3_CHANNEL_TYPE_DMA,
						CHANNEL_NO_WR,  //channel number
						0,  //tcc number
						0); //event queue number
	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS,
						EDMA3_CHANNEL_TYPE_DMA,
						CHANNEL_NO_RD,  //channel number
						1,  //tcc number
						0); //event queue number

	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS,
						EDMA3_CHANNEL_TYPE_DMA,
						CHANNEL_NO_INIT_0,  //channel number
						1,  //tcc number
						0); //event queue number


}


 /*============================================================
  * Function: TdcEnterAutoMode
  * Description: DMA peripheral initialization for TDC7201.
  * This function is used only inside Tdc driver, should not be
  * called outside the tdc driver scope. The function initialize
  * the DMA controller for sequence read of TDC7201 of all the result
  * registers. 9 channels were used, find the detailed information in
  * Configuração_DMA_TDC7201.docx.
  *
  * Para:
  *    >> tdc_handle_t: tdc handle.
  * Return:
  *    >>
  * Change Record:
  *    >> (12/Jan/2018): Creation of the function;
  *    >> (26/Jan/2018): Finished.
  *
  *============================================================*/
  static void TdcEnterAutoMode(tdc_handle_t * handle)
  {
 	tdc_internal_t * ptr_internal = (tdc_internal_t *)handle->ptr_internal;
 	spi_handle_t * spi_handle = ptr_internal->spi_handle;
 	EDMA3CCPaRAMEntry *ptr_param_set, param_set;
 	uint16_t cnt = 0;

     //1 - Initiate the write and read buffer for DMA.
 	ptr_internal->dma.wt_buffer[0] = 0x1000|0x8000;
 	for(cnt = 1;cnt < 19; cnt++) {
     	ptr_internal->dma.wt_buffer[cnt] = 0x10340000;
    }

     //ptr_internal->dma.wt_buffer[20] = 0x0;

 	// 2- Initiate the command buffer for DMA.
     //Disable DMA Event CMD Disable Channel CHANNEL_NO_WR
    ptr_internal->dma.cmds[0] = (0x01 << CHANNEL_NO_WR); //0x00008000;
 	//spi conf. //No cs_hold + CS_SELECT + Start CMD
 	// * [first 16 bits] - value of spi_data1;
 	// * [second 16 bits] - value of StartCMD
 	ptr_internal->dma.cmds[1] = ptr_internal->cfg1|0x01|0x4000|0x10380000;
 	ptr_internal->dma.cmds[2] = ptr_internal->cfg1|0x01|0x4000|0x101c0000;
 	ptr_internal->dma.cmds[3] = ptr_internal->cfg1|0x01|0x4000|0x102c0000;
 	ptr_internal->dma.cmds[4] = ptr_internal->cfg1|0x01|0x4000|0x10340000;


 	ptr_internal->dma.cmds[5] = ((uint32_t)0x9000); //reserved
 	//Enable DMA Event CHANNEL_NO_WR&CHANNEL_NO_RD
 	ptr_internal->dma.cmds[6] = (0x01 << CHANNEL_NO_WR)|(0x01 << CHANNEL_NO_RD);
 	 //SPI conf: CS hold + CS4 - CH2 + Consequent read command
 	// * [first 16 bits] - value of spi_data1;
 	// * [second 16 bits] - value of READ SEQ CMD. 0x10 is address of result regs TDC
 	ptr_internal->dma.cmds[7] = 0x10340000|AUTO_INCREMENT|(0x10<<8);
 	ptr_internal->dma.cmds[8] = 0x10380000|AUTO_INCREMENT|(0x10<<8);
 	ptr_internal->dma.cmds[9] = 0x101c0000|AUTO_INCREMENT|(0x10<<8);
 	ptr_internal->dma.cmds[10] = 0x102c0000|AUTO_INCREMENT|(0x10<<8);

 	//3 - Init DMA
 	EDMA3Init(SOC_EDMA30CC_0_REGS, 0);

 	//4 - DMA channel Request, event Q0 is arragend.
 	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS,
 						EDMA3_CHANNEL_TYPE_DMA,
						CHANNEL_NO_WR,  //channel number
						CHANNEL_NO_WRAUX_1,  //tcc number
 						0); //event queue number
 	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS,
						EDMA3_CHANNEL_TYPE_DMA,
						CHANNEL_NO_WRAUX_1,  //channel number
						CHANNEL_NO_WRAUX_2,  //tcc number
						0); //event queue number
 	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS,
 						EDMA3_CHANNEL_TYPE_DMA,
						CHANNEL_NO_WRAUX_2,  //channel number
						CHANNEL_NO_WRAUX_3,  //tcc number
 						0); //event queue number
 	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS,
 	 					EDMA3_CHANNEL_TYPE_DMA,
						CHANNEL_NO_WRAUX_3,  //channel number
						CHANNEL_NO_WRAUX_3,  //tcc number
 	 					0); //event queue number


 	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS,
 						EDMA3_CHANNEL_TYPE_DMA,
						CHANNEL_NO_RD,  //channel number
						CHANNEL_NO_RD,  //tcc number
 						0); //event queue number

 	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS,
 						EDMA3_CHANNEL_TYPE_DMA,
						CHANNEL_NO_INIT_0,  //channel number
						CHANNEL_NO_INIT_3,  //tcc number
 						0); //event queue number
 	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS,
 						EDMA3_CHANNEL_TYPE_DMA,
						CHANNEL_NO_INIT_3,  //channel number
						CHANNEL_NO_INIT_1,  //tcc number
 						0); //event queue number
 	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS,
 	 					EDMA3_CHANNEL_TYPE_DMA,
						CHANNEL_NO_INIT_1,  //channel number
						CHANNEL_NO_INIT_2,  //tcc number
						0); //event queue number
 	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS,
						EDMA3_CHANNEL_TYPE_DMA,
						CHANNEL_NO_INIT_2,  //channel number
						CHANNEL_NO_INIT_2,  //tcc number
						0); //event queue number

 	//5 - Init of the param sets for all the DMA Channel used.
 	//5.1 - Init the param_set for CHANNEL WR
 	ptr_param_set = &param_set;
 	ptr_param_set->srcAddr = (uint32_t)&ptr_internal->dma.wt_buffer[1]; //write buffer+1 (16bit)
 	ptr_param_set->destAddr = spi_handle->base_addr + 0x3c;//0x38; //address of SPIDAT0
 	ptr_param_set->bCnt = 19;
 	ptr_param_set->srcBIdx = 4;
 	ptr_param_set->destBIdx = 0;
 	ptr_param_set->srcCIdx = 0;
 	ptr_param_set->destCIdx =  0;
 	ptr_param_set->bCntReload = 19; //no reload needed
 	ptr_param_set->opt = 0x00400200|(CHANNEL_NO_WRAUX_1<<12);
 	ptr_param_set->linkAddr = 0xffff; //--->PARAM2
 	ptr_param_set->aCnt = 4; //32 bits
 	ptr_param_set->cCnt = 1; //ONE_READ_LENGTH // -------changed------ from 1 to 4
 	//Save this param set in cmds for future use
 	ptr_internal->dma.cmds[11] = ptr_param_set->opt;
	ptr_internal->dma.cmds[12] = ptr_param_set->srcAddr;
	ptr_internal->dma.cmds[13] = ptr_param_set->aCnt + (ptr_param_set->bCnt << 16);
	ptr_internal->dma.cmds[14] =	ptr_param_set->destAddr;
	ptr_internal->dma.cmds[15] = ptr_param_set->srcBIdx + (ptr_param_set->destBIdx << 16);
	ptr_internal->dma.cmds[16] = ptr_param_set->linkAddr + (ptr_param_set->bCntReload << 16);
	ptr_internal->dma.cmds[17] = (ptr_param_set->srcCIdx&0x0000ffff) + (ptr_param_set->destCIdx << 16);
	ptr_internal->dma.cmds[18] = ptr_param_set->cCnt;

 	//5.2 - WR AUXILIAR 1: just to waste some time....
 	ptr_param_set->srcAddr = (uint32_t)&ptr_internal->dma.cmds[11];
 	ptr_param_set->destAddr = (uint32_t)&ptr_internal->dma.waste;
 	ptr_param_set->bCnt = 50;
 	ptr_param_set->srcBIdx = 0;
 	ptr_param_set->destBIdx = 0;
 	ptr_param_set->srcCIdx = 0;
 	ptr_param_set->destCIdx =  0;
 	ptr_param_set->bCntReload = 50; //no reload needed
	ptr_param_set->opt = 0x00c00104|(CHANNEL_NO_WRAUX_2<<12);
	ptr_param_set->linkAddr = 0xffff;
	ptr_param_set->aCnt = 2; //16 bits
	ptr_param_set->cCnt = ONE_READ_LENGTH;//ONE_READ_LENGTH;
 	EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, CHANNEL_NO_WRAUX_1, ptr_param_set);

 	//5.3 - Init para_set for WR AUXILIAR 2
 	ptr_param_set->srcAddr = (uint32_t)&ptr_internal->dma.cmds[1]; //write buffer+1 (16bit)
 	ptr_param_set->destAddr = spi_handle->base_addr + 0x3c; //address of SPI DAT0
 	ptr_param_set->bCnt = 4;//------Changed------//from 1 to 4:
 	ptr_param_set->srcBIdx = 4;//------Changed------//from 0 to 4:
 	ptr_param_set->destBIdx = 0;
 	ptr_param_set->srcCIdx = -12; //----Changed-----//from 0 to -12
 	ptr_param_set->destCIdx = 0;
 	ptr_param_set->bCntReload = 4; //------changed-----//from 1 to 4:
 	ptr_param_set->opt = 0x00c00200|(CHANNEL_NO_WRAUX_3<<12);
 	ptr_param_set->linkAddr = 0xffff;
 	ptr_param_set->aCnt = 4; //32 bits
 	ptr_param_set->cCnt = ONE_READ_LENGTH/4; //-------Changed--------//from ONE_READ_LENGTH to ONE_READ_LENGTH/4
 	EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, CHANNEL_NO_WRAUX_2, ptr_param_set);

 	//5.4 - Init the param_set for WR AUXILIAR 3
 	ptr_param_set->srcAddr = (uint32_t)&ptr_internal->dma.cmds[0]; //write buffer+1 (16bit)
 	ptr_param_set->destAddr = SOC_EDMA30CC_0_REGS+0x1028; //address of DMA EECR
 	ptr_param_set->bCnt = 1;
 	ptr_param_set->srcBIdx = 0;
 	ptr_param_set->destBIdx = 0;
 	ptr_param_set->srcCIdx = 0;
 	ptr_param_set->destCIdx =  0;
 	ptr_param_set->bCntReload = 1; //no reload needed
 	ptr_param_set->opt = 0x00000100|(CHANNEL_NO_WRAUX_3<<12);
 	ptr_param_set->linkAddr = 0xffff;
 	ptr_param_set->aCnt = 4; //32 bits
 	ptr_param_set->cCnt = ONE_READ_LENGTH;
 	EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, CHANNEL_NO_WRAUX_3, ptr_param_set);

 	//5.5 - Init the para_set for CHANNEL RD
 	ptr_param_set->srcAddr = spi_handle->base_addr + 0x40; //address of SPIBUF
 	ptr_param_set->destAddr = (uint32_t)&ptr_internal->dma.rd_buffer[0]; //write buffer+1 (16bit)
 	ptr_param_set->bCnt = 21*ONE_READ_LENGTH;
 	ptr_param_set->srcBIdx = 0;
 	ptr_param_set->destBIdx = 2;
 	ptr_param_set->srcCIdx = 0;
 	ptr_param_set->destCIdx =  0;
 	ptr_param_set->bCntReload = 21*ONE_READ_LENGTH; //no reload needed
 	ptr_param_set->opt = 0x00000100|(CHANNEL_NO_RD<<12);
 	ptr_param_set->linkAddr = 0xffff;
 	ptr_param_set->aCnt = 2;
 	ptr_param_set->cCnt = 1;
 	EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, CHANNEL_NO_RD, ptr_param_set);

 	//5.6 - Init the param_set for CHANNEL INIT 0
 	ptr_param_set->srcAddr = (uint32_t)&ptr_internal->dma.cmds[11]; //write buffer+1 (16bit)
 	ptr_param_set->destAddr = SOC_EDMA30CC_0_REGS + 0x4000 + CHANNEL_NO_WR*0x20; //address of DMA Param15
 	ptr_param_set->bCnt = 0x10;
 	ptr_param_set->srcBIdx = 2;
 	ptr_param_set->destBIdx = 2;
 	ptr_param_set->srcCIdx = 0;
 	ptr_param_set->destCIdx =  0;
 	ptr_param_set->bCntReload = 0x10; //no reload needed
 	ptr_param_set->opt = 0x00c00104|(CHANNEL_NO_INIT_3<<12); //
 	ptr_param_set->linkAddr = 0xffff;
 	ptr_param_set->aCnt = 2; //32 bits
 	ptr_param_set->cCnt = ONE_READ_LENGTH;//ONE_READ_LENGTH;
 	EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, CHANNEL_NO_INIT_0, ptr_param_set);

 	//5.7 - Init the param_set of CHANNEL INIT 1
	ptr_param_set->srcAddr = (uint32_t)&ptr_internal->dma.cmds[6]; //write buffer+1 (16bit)
	ptr_param_set->destAddr = SOC_EDMA30CC_0_REGS + 0x1040; //0x308;
	ptr_param_set->bCnt = 2;
	ptr_param_set->srcBIdx = 0;
	ptr_param_set->destBIdx = -0xd38;
	ptr_param_set->srcCIdx = 0;
	ptr_param_set->destCIdx = 0;
	ptr_param_set->bCntReload = 2;
	ptr_param_set->opt = 0x00c00204|(CHANNEL_NO_INIT_2<<12);
	ptr_param_set->linkAddr = 0xffff; //--->PARAM2
	ptr_param_set->aCnt = 4; //32 bits
	ptr_param_set->cCnt = ONE_READ_LENGTH; //ONE_READ_LENGTH
	EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, CHANNEL_NO_INIT_1, ptr_param_set);

 	//5.8 - Init the param_set of CHANNEL INIT 2
 	ptr_param_set->srcAddr = (uint32_t)&ptr_internal->dma.cmds[6]; //write buffer+1 (16bit)
 	ptr_param_set->destAddr = SOC_EDMA30CC_0_REGS+0x1030; //address of DMA EECR
 	ptr_param_set->bCnt = 1;
 	ptr_param_set->srcBIdx = 0;
 	ptr_param_set->destBIdx = 0;
 	ptr_param_set->srcCIdx = 0;
 	ptr_param_set->destCIdx =  0;
 	ptr_param_set->bCntReload = 1; //no reload needed
 	ptr_param_set->opt = 0x00000200|(CHANNEL_NO_INIT_2<<12);
 	ptr_param_set->linkAddr = 0xffff;
 	ptr_param_set->aCnt = 4; //32 bits
 	ptr_param_set->cCnt = ONE_READ_LENGTH;
 	EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, CHANNEL_NO_INIT_2, ptr_param_set);

 	//5.9 - Init the param_set of CHANNEL INIT 3
 	ptr_param_set->srcAddr = (uint32_t)&ptr_internal->dma.cmds[7]; //write buffer+1 (16bit)
 	ptr_param_set->destAddr = spi_handle->base_addr + 0x3c; //address of DMA EECR
 	ptr_param_set->bCnt = 4; //-------Changed--------: from 1 to 4
 	ptr_param_set->srcBIdx = 4; //-------Changed--------: from 0 to 4
 	ptr_param_set->destBIdx = 0;
 	ptr_param_set->srcCIdx = -12; //-------changed------: from 0 to -12
 	ptr_param_set->destCIdx =  0;
 	ptr_param_set->bCntReload = 4; //-------Changed--------: from 1 to 4
 	ptr_param_set->opt = 0x00c00200|(CHANNEL_NO_INIT_1<<12); //32bits
 	ptr_param_set->linkAddr = 0xffff;
 	ptr_param_set->aCnt = 4; //32 bits
 	ptr_param_set->cCnt = ONE_READ_LENGTH/4;//-------Changed--------: from ONE_READ_LENGTH to ONE_READ_LENGTH/4
 	EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, CHANNEL_NO_INIT_3, ptr_param_set);

 	//6 - Clear some flags: not necessary
 	EDMA3ClrMissEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_WR);
 	EDMA3ClrMissEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_RD);
 	EDMA3ClrMissEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_INIT_0);
 	EDMA3ClrMissEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_WRAUX_2);
 	EDMA3ClrMissEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_WRAUX_3);
 	EDMA3ClrEvt(SOC_EDMA30CC_0_REGS,CHANNEL_NO_WR);
}


  /*============================================================
   * Function: TdcDmaAutoStart
   * Description: This function:
   * (1) Clear the event flag and miss event flag
   * (2) Enable the SPI DMA event, which will starts the dma transfer.
   *
   * Para:
   *    >> tdc_handle_t : a tdc7201 handle
   * Return:
   *     >>
   * Change Record:
   *		>> (12/Jan/2018): Creation of the function;
   *		>> (26/Jan/2018): finished.
   *
   *============================================================*/
 void DrvTdcDmaAutoStart(tdc_handle_t * handle)
  {
	 tdc_internal_t * ptr_internal = (tdc_internal_t *)handle->ptr_internal;

	 //set current channel to channel 0
	 ptr_internal->curr = ptr_internal->ch0;

	 TdcEnterAutoMode(handle);
	 DrvTdcStart(handle);
	 ptr_internal->spi_handle->CsHold(ptr_internal->spi_handle,true,
			 ptr_internal->curr->set_cs_cmd);
	 GPIOBankIntEnable(SOC_GPIO_0_REGS, 0);
	 SPIIntEnable(ptr_internal->spi_handle->base_addr, SPI_DMA_REQUEST_ENA_INT);

  }

 /*============================================================
  * Function: TdcInt
  * Description: This function only needed for test.
  * It generate an event for CHANNEL_NO_INIT_0 in order
  * to begin all the dma action.
  * TODO: SHOULD BE DELETED.
  * Para:
  *    >> tdc_handle_t : a tdc7201 handle
  * Return:
  *     >>
  * Change Record:
  *		>> (12/Jan/2018): Creation of the function;
  *		>> (26/Jan/2018): finished.
  *
  *============================================================*/
  void TdcInt(tdc_handle_t *handle)
  {
	  EDMA3EnableTransfer(SOC_EDMA30CC_0_REGS, CHANNEL_NO_INIT_0, EDMA3_TRIG_MODE_MANUAL);
  }

  /*============================================================
   * Function: Calc
   * Description: Calculate function to convert results from
   * TDC reg values to TOF values.
   *
   * Para:
   *    >> tdc_handle_t : a tdc7201 handle
   * Return:
   *     >>
   * Change Record:
   *		>> (12/Jan/2018): Creation of the function;
   *		>> (26/Jan/2018): finished.
   *
   *============================================================*/
  static void TdcCalc(tdc_handle_t * handle, int index, uint32_t *result)
  {
 	  tdc_internal_t * ptr_internal;
	  ptr_internal = (tdc_internal_t *) handle->ptr_internal;
	  uint32_t timer1, counter1, timer2, counter2, timer3, counter3, timer4;
	  uint32_t counter4, timer5, counter5, timer6, calibration1, calibration2;

	  //Mark that the measuremente is end.
	  uint16_t *data = &ptr_internal->dma.rd_buffer[index];
	  uint32_t normal_lsb, cal_count;

	  //TdcReadRegSeqDma(handle);

	  timer1 = (((uint32_t)data[0]&0xff)<<16)+
							(data[1]&0xffff);
	  counter1 = ((uint32_t)data[2]<<16)+
								(data[3]&0xff00)>>8;
	  timer2 = (((uint32_t)data[3]&0xff)<<16)+
			(data[4]&0xffff);
	  counter2 = ((uint32_t)data[5]<<16)+
		(data[6]&0xff00)>>8;
	  timer3 = (((uint32_t)data[6]&0xff)<<16)+
			(data[7]&0xffff);
	  counter3 = ((uint32_t)data[8]<<16)+
			(data[9]&0xff00)>>8;
	  timer4 = (((uint32_t)data[9]&0xff)<<16)+
			(data[10]&0xffff);
	  counter4 = ((uint32_t)data[11]<<16)+
				(data[12]&0xff00)>>8;
	  timer5 = (((uint32_t)data[12]&0xff)<<16)+
			(data[13]&0xffff);
	  counter5 = ((uint32_t)data[14]<<16)+
				(data[15]&0xff00)>>8;
	  timer6 = (((uint32_t)data[15]&0xff)<<16)+
			(data[16]&0xffff);
	  calibration1 = ((uint32_t)data[17]<<16)+
				(data[18]&0xff00)>>8;
	  calibration2 = (((uint32_t)data[18]&0xff)<<16)+
			(data[19]&0xffff);

	  cal_count = (calibration2-calibration1)/(ptr_internal->cali_period-1);
	  normal_lsb = TDC_CLK_PERIOD/cal_count;
	  *result++ = normal_lsb*(timer1-timer2)+counter1*TDC_CLK_PERIOD;
	  *result++ = normal_lsb*(timer2-timer3)+(counter2-counter1)*TDC_CLK_PERIOD;
	  *result++ = normal_lsb*(timer3-timer4)+(counter3-counter2)*TDC_CLK_PERIOD;
	  *result++ = normal_lsb*(timer4-timer5)+(counter4-counter3)*TDC_CLK_PERIOD;
	  *result++ = normal_lsb*(timer5-timer6)+(counter5-counter4)*TDC_CLK_PERIOD;
  }


  /*============================================================
   * Function: DrvTdcSetComm
   * Description: Set communication handle
   *
   * Para:
   *    >> tdc_handle_t : a tdc7201 handle
   *    >> void: a communication handle
   * Return:
   *     >>
   * Change Record:
   *		>> (07/Fev/2018): Creation of the function;
   *
   *============================================================*/
  void DrvTdcSetComm(tdc_handle_t * handle, void * comm)
  {
	  handle->ptr_internal = malloc(sizeof(comm));
	  handle->ptr_internal = comm;
//		tdc_internal_t * ptr_internal =
//	  			(tdc_internal_t *)handle->ptr_internal;
//		ptr_internal->spi_handle = (spi_handle_t *)comm;
  }


  /*============================================================
    * Function: DrvTdcNextChannel
    * Description: Set to next channel
    *
    * Para:
    *    >> tdc_handle_t : a tdc7201 handle
    * Return:
    *     >>
    * Change Record:
    *		>> (01/Mar/2018): Creation of the function;
    *
    *============================================================*/
   void DrvTdcNextChannel(tdc_handle_t * handle)
   {
 		tdc_internal_t * ptr_internal =
 	  			(tdc_internal_t *)handle->ptr_internal;
 		ptr_internal->curr = ptr_internal->curr->next;
   }
