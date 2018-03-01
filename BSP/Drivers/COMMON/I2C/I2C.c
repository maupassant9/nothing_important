/*
* FileName: I2C.c
* Author: Dong Xia
* This is source file of I2C.
* I2C driver currently only support Master Read mode.
*
* Change Records:
*      >> (01/Fev/2018): Create file
*
*/

/********************************************
* Include 
********************************************/
#include <stdlib.h>
#include "hw_types.h"
#include "include/hw/soc_OMAPL138.h"
#include "include/hsi2c.h"
#include "include/psc.h"
#include "hw_syscfg0_OMAPL138.h"

#include "../BSP/type.h"
#include "../BSP/conf.h"
#include "../BSP/connector.h"
#include "../BSP/board.h"

/********************************************
* Internal Function Declaration
********************************************/
#define I2C_RX_READY 0x08
#define I2C_TX_READY 0x10
#define I2C_MODE_STP 0x0800
#define I2C_MODE_BC_8 0x000
#define I2CInterruptVectorGet(addr) HWREG(addr + 0x8)
#define I2CInterruptVectorClr(addr) HWREG(addr + 0x8) &= ~I2C_RX_READY;
#define I2CSetBitLenAs8Bits(addr) HWREG(addr + 0x8) = HWREG(addr + 0x8)&0xf8
#define I2CSetMode(addr,val) HWREG(addr+0x24) = HWREG(addr+0x24)|val
#define I2CClrMode(addr,val) HWREG(addr+0x24) = HWREG(addr+0x24)|val

/********************************************
* Internal Types and Variables
********************************************/
typedef struct{
    uint32_t base_addr;
    uint8_t master_addr;
    uint8_t slave_addr;
    uint8_t mode;
} i2c_internal_t;
/********************************************
* External Variables
********************************************/
static void I2cSlaveAddrSet(i2c_handle_t * handle);
static void I2cPinsInit(i2c_handle_t * handle);

/********************************************
* Functions
********************************************/
/*============================================================
 * Function:  DrvI2cInit()
 * Description:  Initiate the I2C module, configurate the i2c
 * module registers. I2C of OMAPL138 is used as:
 *      Data length: 8 bits
 * Para:
 *    >> i2c_handle_t *: handle for i2c
 * Return:
 *     >>
 * Change Record:
 *		>> (04/Jan/2018): Creation of the function;
 *============================================================*/
 void DrvI2cInit(i2c_handle_t * handle, i2c_conf_t * ptr_conf)
 {
     handle->ptr_internal = (void *) malloc(sizeof(i2c_internal_t));
     i2c_internal_t * ptr_internal = (i2c_internal_t *) handle->ptr_internal;

     handle->id = ptr_conf->id;
     //Enable I2C clock from PSC module
     if(handle->id == 1)
     {
        PSCModuleControl(SOC_PSC_1_REGS,HW_PSC_I2C1,
            PSC_POWERDOMAIN_ALWAYS_ON,PSC_MDCTL_NEXT_ENABLE);
        ptr_internal->base_addr = SOC_I2C_1_REGS;
     }
     else
     {
         ptr_internal->base_addr = SOC_I2C_0_REGS;
     }

     //Place I2C in reset
     I2CMasterDisable(ptr_internal->base_addr);

     //Configure I2C pins
     I2cPinsInit(handle);

     //Set i2c bus speed
     if(handle->id == 1)
        I2CMasterInitExpClk(SOC_I2C_1_REGS,
                            I2C_1_FREQ,
                            8000000, //fixed value (6.7M - 13.3MHz)
                            ptr_conf->speed);
    else
        I2CMasterInitExpClk(SOC_I2C_0_REGS,
                            I2C_0_FREQ,
                            8000000,
                            ptr_conf->speed);
    //set i2c master mode: master receiver
    //                     7 bits slave addr
    //                     with stop and normal mode.
    I2CMasterControl(ptr_internal->base_addr,
    					I2C_CFG_MST_RX|I2C_CFG_7BIT_SLAVE_ADDR);

    I2CSetBitLenAs8Bits(ptr_internal->base_addr);

    //Set own address: slave address here set to 0x11, should be changed.
    ptr_internal->slave_addr = ptr_conf->addr;
    handle->addr = ptr_conf->addr;
    I2CMasterSlaveAddrSet(ptr_internal->base_addr, handle->addr);
//    if(handle->id == 1)
//        I2COwnAddressSet(ptr_internal->base_addr, ptr_conf->addr, I2C_OWN_ADDR_1);
//    else
//        I2COwnAddressSet(ptr_internal->base_addr, ptr_conf->addr, I2C_OWN_ADDR_0);

    //Configure ICMDR
     I2CMasterEnable(ptr_internal->base_addr);
}

/*============================================================
 * Function:  I2cSlaveAddrSet()
 * Description:  Set the DrvI2CSlaveAddr in i2c register and
 * change slave_addr in internal memeory.
 * Para:
 *    >> i2c_handle_t *: handle for i2c
 *    >> uint8_t: address of I2C
 * Return:
 *     >>
 * Change Record:
 *		>> (04/Jan/2018): Creation of the function;
 *============================================================*/
static void I2cSlaveAddrSet(i2c_handle_t * handle)
{
    //set the slave addr if necessary
    i2c_internal_t *ptr_internal = (i2c_internal_t *)handle->ptr_internal;

    if(ptr_internal->slave_addr != handle->addr)
    {
        ptr_internal->slave_addr = handle->addr;
        I2CMasterSlaveAddrSet(ptr_internal->base_addr, handle->addr);
    }
}


/*============================================================
 * Function:  DrvI2cRead()
 * Description:  Set the DrvI2CSlaveAddr in i2c register and
 * change slave_addr in internal memeory.
 * //TODO: inapproporate infinite waiting, need more work.
 *
 * Para:
 *    >> i2c_handle_t *: handle for i2c
 *    >> uint8_t: address of I2C
 *    >> uint8_t: pointer of memory for rsv data
 *    >> uint8_t: length in bytes of data to be rsved.
 * Return:
 *     >>
 * Change Record:
 *		>> (04/Jan/2018): Creation of the function;
 *
 *============================================================*/
void DrvI2cRead(i2c_handle_t * handle,
                uint8_t *rsv_data,
                uint8_t len)
{
    i2c_internal_t *ptr_internal = (i2c_internal_t *)handle->ptr_internal;
    uint8_t cnt = 0;
    //wait until I2C0 is ready.
    while(I2CMasterBusBusy(ptr_internal->base_addr));
    //Set to Master Read Mode
    I2CMasterControl(ptr_internal->base_addr,
                    I2C_CFG_MST_RX|I2C_CFG_7BIT_SLAVE_ADDR);
    I2CSetMode(ptr_internal->base_addr,I2C_MODE_STP);
    //Set the slave address.
    I2cSlaveAddrSet(handle);
    //Set the data length
    I2CSetDataCount(ptr_internal->base_addr, len);
    //Start the transfer
    I2CMasterStart(ptr_internal->base_addr);
    //wait until I2C send everything
    for(;cnt < len; cnt++)
    {
        while(!(I2CInterruptVectorGet(ptr_internal->base_addr)&I2C_RX_READY));
        //I2CInterruptVectorClr(ptr_internal->base_addr);
        *rsv_data++ = (uint8_t)I2CMasterDataGet(ptr_internal->base_addr);
    }
    while(I2CMasterBusBusy(ptr_internal->base_addr));
}


/*============================================================
 * Function:  DrvI2cWrite()
 * Description:  Set the DrvI2CSlaveAddr in i2c register and
 * change slave_addr in internal memeory.
 * //TODO: inapproporate infinite waiting, need more work.
 *
 * Para:
 *    >> i2c_handle_t *: handle for i2c
 *    >> uint8_t: starting addr of At24C02
 *    >> uint8_t: address of I2C
 *    >> uint8_t: pointer of memory for write data
 *    >> uint8_t: length in bytes of data to be write.
 * Return:
 *     >>
 * Change Record:
 *		>> (04/Jan/2018): Creation of the function;
 *
 *============================================================*/
void DrvI2cWrite(i2c_handle_t * handle,
				uint8_t wr_stx_addr,
                uint8_t *wr_data,
                uint8_t len)
{
	uint8_t cnt = 0;
    i2c_internal_t *ptr_internal = (i2c_internal_t *)handle->ptr_internal;
    //wait until I2C0 is ready.
    while(I2CMasterBusBusy(ptr_internal->base_addr));
    //Set to Master Write Mode
    I2CMasterControl(ptr_internal->base_addr,
                    I2C_CFG_MST_TX|I2C_CFG_7BIT_SLAVE_ADDR);
	I2CSetMode(ptr_internal->base_addr,I2C_MODE_STP);
    //Set the slave address.
    I2cSlaveAddrSet(handle);
    //Set the data length
    I2CSetDataCount(ptr_internal->base_addr, len+1);
    //Start the transfer
    I2CMasterStart(ptr_internal->base_addr);

    while(!(I2CInterruptVectorGet(ptr_internal->base_addr)&I2C_TX_READY));
    //I2CInterruptVectorClr(ptr_internal->base_addr);
    I2CMasterDataPut(ptr_internal->base_addr, wr_stx_addr);
    //wait until I2C send everything
    for(;cnt < len; cnt++)
    {
        while(!(I2CInterruptVectorGet(ptr_internal->base_addr)&I2C_TX_READY));
        //I2CInterruptVectorClr(ptr_internal->base_addr);
        I2CMasterDataPut(ptr_internal->base_addr, wr_data[cnt]);
    }
    while(I2CMasterBusBusy(ptr_internal->base_addr));
}

/*============================================================
 * Function:  I2cPinsInit()
 * Description: Configure the I2C pin mutex, set to i2c function
 *
 * Para:
 *    >> i2c_handle_t *: handle for i2c
 * Return:
 *     >>
 * Change Record:
 *		>> (07/Jan/2018): Creation of the function;
 *
 *============================================================*/
static void I2cPinsInit(i2c_handle_t * handle)
{
    uint32_t save_pin_mux;

    if(handle->id == 1)
    {
        save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(I2C1_PINS_MUX_NUM)) &
					  ~I2C1_PINS_MUX_LOC);
	    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(I2C1_PINS_MUX_NUM)) =
					 (I2C1_PINS_MUX_VAL | save_pin_mux);
    }
    else
    {
        save_pin_mux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(I2C0_PINS_MUX_NUM)) &
					  ~I2C0_PINS_MUX_LOC);
	    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(I2C0_PINS_MUX_NUM)) =
					 (I2C0_PINS_MUX_VAL | save_pin_mux);
    }
}
