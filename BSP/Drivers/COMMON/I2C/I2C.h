
/*
* FileName:
* Author: Dong Xia
* This is head file of
*
* Change Records:
*      >> (01/Fev/2017): Creation of file
*
*/


#ifndef BSP_DRIVERS_COMMON_I2C_I2C_H_
#define BSP_DRIVERS_COMMON_I2C_I2C_H_

/********************************************
* Include
********************************************/
#include "../BSP/connector.h"

/********************************************
* Macro
********************************************/
//#define I2C_ADDR 0x01 //master I2C addr
//#define I2C_SPEED 400000 //speed for both i2c0 e i2c1

//Value for internal memory variable: mode
//I2C Read mode
#define I2C_MODE_READ 0x0
#define I2C_MODE_WRITE 0x1

/********************************************
* Type definition
********************************************/


/********************************************
* Function prototype
********************************************/
/*============================================================
 * Function:  DrvI2cInit()
 * Usage:  This function initiate the I2c with configuration
 * given by i2c_conf_t structure.
 * I2C of OMAPL138 is used as:
 *      Master reciever with addr = (I2C0) 0x00, (I2C1) 0x01
 *      Data length: 8 bits
 * Para:
 *    >> i2c_handle_t *: handle for i2c
 * Return:
 *     >>
 * Change Record:
 *		>> (04/Jan/2018): Creation of the function;
 *============================================================*/
 void DrvI2cInit(i2c_handle_t * handle, i2c_conf_t * ptr_conf);


/*============================================================
 * Function:  DrvI2cRead()
 * Description:  Read data from I2C.
 * Para:
 *    >> i2c_handle_t *: handle for i2c
 *    >> uint8_t: pointer of memory for rsv data
 *    >> uint8_t: length in bytes of data to be rsved.
 * Return:
 *     >>
 * Change Record:
 *		>> (04/Jan/2018): Creation of the function;
 *============================================================*/
void DrvI2cRead(i2c_handle_t * handle, uint8_t *rsv_data, uint8_t len);

/*============================================================
 * Function:  DrvI2cWrite()
 * Description:  Set the DrvI2CSlaveAddr in i2c register and
 * change slave_addr in internal memeory.
 * //TODO: inapproporate infinite waiting, need more work.
 *
 * Para:
 *    >> i2c_handle_t *: handle for i2c
 *    >> uint8_t: pointer of memory for write data
 *    >> uint8_t: length in bytes of data to be write.
 * Return:
 *     >>
 * Change Record:
 *		>> (04/Jan/2018): Creation of the function;
 *
 *============================================================*/
void DrvI2cWrite(i2c_handle_t * handle, uint8_t wr_stx_addr,
				uint8_t *wr_data, uint8_t len);


#endif /* BSP_DRIVERS_COMMON_I2C_I2C_H_ */
