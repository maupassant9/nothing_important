/*
 * connector.h
 *
 *  Created on: 27 de nov de 2017
 *  Author: dong.x
 *  Change Records:
 *     >>(10/Dec/2017): file created;
 *     >>(13/Dec/2017): uart configuration struture added.
 *     >>(20/Dec/2017): pwm configuration structure added.
 *     >>(21/Dec/2017): spi added.
 */

#ifndef BSP_DRIVERS_CONNECTOR_H_
#define BSP_DRIVERS_CONNECTOR_H_

#include "type.h"
#include "conf.h"


/* ===============PWM=================*/
// 1 - Pwm Constant Definition


// 2 - Pwm config structure
//  |<-1-->|< 2>|
//          ____                           ____
//         |    |                         |    |
//  _______|    |__________________*______|    |____________________
//  |<--------------3------------->|
// 1. delay (ns)
// 2. pulse_width (ns)
// 3. 1/freq.
typedef struct {
    uint32_t freq;
    uint32_t delay;
    uint32_t pulse_width;

    uint32_t in_freq; //input frequency of pwm module: sysclk2
    uint8_t clk_div;// input clock divider value
} pwm_conf_t;

// 3 - Pwm handler
struct pwm_struct_t{
    //Variable used to save internal datas
    void * ptr_pwm_data;
    //Used for semaphore
    void * ptr_lock;

    //PWM function pointers
    void (*Init)(struct pwm_struct_t *, pwm_conf_t *);
    bool_t (*SetPulseWidth)(struct pwm_struct_t *, uint32_t);
    bool_t (*SetDelay)(struct pwm_struct_t *, uint32_t);
    bool_t (*SetFreq)(struct pwm_struct_t *, uint32_t);
    bool_t (*Start)(struct pwm_struct_t *);
    bool_t (*Stop)(struct pwm_struct_t *);

};
typedef struct pwm_struct_t pwm_handle_t;


/* ===============Uart=================*/
// 1 - Uart Constant Definition
#define DEV_UART 0x11 //TODO: Delete in the future.

// 2 - Uart config structure
typedef struct{
    //uart_id = 0: uart0 (default)
    //uart_id = 1: uart1
    //uart_id = 2: uart2
    uint8_t uart_id;

    //input frequency of usart
    uint32_t input_freq;
    //Baud Rate. (9600, 115200, etc).
    uint32_t baud_rate;
    //Over sample rate: 0 (default) = 16, any positive value = 13
    uint8_t over_sample_rate;
    //transfer data length:
    //                      1 - 5 bits, 2 - 6 bits,
    //                      3 - 7 bits, 0 - 8 bits: default
    uint32_t data_len;
    //no priority :
    // - stop bits:
    //      0 (1 bit): default
    //      1 (1 bit)
    //      2 Multiple Bit
    //with odd priority:
    // - stop bits:
    //      5 (1 bit)
    //      6 Multiple bitin
    //with even priority:
    // - stop bits:
    //      9 (1 bit)
    //      10 Multiple bit
    uint8_t priority_stop_bits;
    //fifo level:
    // 0 -- no fifo used
    // 1 -- 1 byte
    // 2 -- 4 bytes
    // 3 -- 8 bytes
    // 4 -- 14 bytes
    uint8_t fifo_level;
    // interrupt setting
    // X X X X X X X X (default: 0)
    // 0 -- interrupt Disable
    // 1 -- interrupt Enable
    //(bit 1): Modem Status Interrupt
    //(bit 2): Line Status Interrupt
    //(bit 3): Tx empty Interrupt
    //(bit 4): Reveiver Data Avaliable and Character timeout
    //(bit 5-8): reserved
    uint8_t interrupt_en_bits;
} uart_conf_t;

// 3 - Uart handler
struct uart_struct_t {
    //variable used to save internal datas
    void * ptr_uart_data;
    // Used for Semaphore
    void * ptr_lock;

    //conf
    uart_conf_t *ptr_cfg; //TODO: should be deleted in future.
    //Start Uart function pointer
    void (*Init)(struct uart_struct_t *, uart_conf_t *);
    int32_t (*ReadChar) (struct uart_struct_t *);
    uint8_t (*ReadCharBlock)(struct uart_struct_t *);
    void (*WriteCharBlock) (struct uart_struct_t * , uint8_t);
    void (*WriteString) (struct uart_struct_t * , char *);
    bool_t (*WriteChar)(struct uart_struct_t *, uint8_t);
    bool_t (*HasChar) (struct uart_struct_t *);
    bool_t (*IsTxRdy) (struct uart_struct_t *);
    void (*Disable) (struct uart_struct_t *);
};
typedef struct uart_struct_t uart_handle_t;

/* ==============SPI==================*/
// 2 - SPI config structure
typedef struct {
    //id of SPI:
    // id = 0: SPI_0
    // id = 1: SPI_1
    uint8_t id;
    //Programmed length of SPI serial bit stream
    //Should be 2 - 16 bits.
    uint8_t data_length;
    //CS Pin used
    // CS pins: XXXXXXXX
    //          ----1---: bit[n] = 1, CS_n is used.
    //          ----0---: bit[n] = 0. CS_n is not used.
    // only CS_0 ~ CS_5 is supported
    uint8_t cs_pins;
    //SPI Clk frequency
    uint32_t clk_freq;
    //SPI clk polarity
    // 0: Initiate state is low, Data is output on rising
    //    edge of clk, input on falling edge; (POL=0,PHA=0)
    // 1: Initiate state is low, Data output one half-cycle
    //    before the first rising edge and on subsequente falling
    //    edge, input data on rising edge of clk.(POL=0,PHA=1)
    // 2: Data is output on falling edge, input on rising edge (POL=1,PHA=0)
    // 3: Data output one half-cycle before the first falling
    //    edge, and on subsequent rising edge, input data on falling
    //    edge.(POL=1,PHA=1)
    uint8_t clk_polarity_phase;
    //SPI CS delay
    uint8_t cs_delay;
    //Shift out direction
    // true: shift out LSB first
    // false: shift out MSB first
    bool_t shift_lsb_first;

} spi_conf_t;

// 3 - spi handler
struct spi_struct_t{
    uint8_t dev_id;
    uint32_t base_addr;
	//Variable used to save internal datas
    void * ptr_spi_data;
    //Used for semaphore
    void * ptr_lock;

    // TODO: Delete the following 4 lines.
    // uint8_t * ptr_buffer_rd;
    // uint32_t buffer_rd_sz;

    // uint8_t * ptr_buffer_wt;
    // uint32_t buffer_wt_sz;

    //SPI function pointers
    void (*Init)(struct spi_struct_t *, spi_conf_t*);
    uint32_t (*Read)(struct spi_struct_t *);
    //uint8_t (*ReadDma)(struct spi_struct_t *);
    void (*Write)(struct spi_struct_t *, uint32_t);
    //uint8_t (*WriteDma)(struct spi_struct_t *);
    //void (*AttachBuffer)(struct spi_struct_t *, bool_t,
    //                    void *,uint32_t);
    void (*SetCs)(struct spi_struct_t *, uint8_t);
    void (*CsHold)(struct spi_struct_t *, bool_t, uint8_t);


};
typedef struct spi_struct_t spi_handle_t;

/* ===============TDC=================*/
// 1 - TDC Constant Definition
// 2 - TDC config structure
typedef struct {
    // Device No.
    // Valid value: 0, 1, 2, 3
    uint8_t dev_id;
    //input frequency of TDC
    uint32_t in_freq;
    // Time out value
    uint8_t time_out;
    // multiple cycle aging
    // The value of measurement cycle
    // on multi-cycle averaging mode.
    // the valid number is:
    // 1, 4, 8, 16, 32, 64, 128.
    uint8_t multi_cycle_aging;
    // Stop number:
    // the valid stop number is:
    // 1,2,3,4,5.
    uint8_t stop_number;
    // Start and stop edge polarity
    bool_t is_rising_edge;
    // SPI handle pointer
    // if = null: keep unchange
    // else: change the spi_handle
    spi_handle_t * ptr_spi_handle;

    // CS pin value
    // for example, if this device connect with
    // spi CS4, then this value is 0x0100
    //uint8_t cs;

} tdc_conf_t;


// 3 - tdc handler
typedef struct tdc_struct_t{
    void * ptr_internal;
    uint8_t dev_id;
    //functions for tdc
    void (*Init)(struct tdc_struct_t *, void *);
    //Send start cmd to start measurement
    void (*Start)(struct tdc_struct_t *);
    //Get results from TDC7201
    void (*GetResult)(struct tdc_struct_t *, uint32_t *);

    //Enable tdc and Disable TDC
    void (*Enable)(struct tdc_struct_t *);
    void (*Disable)(struct tdc_struct_t *);

    void (*EnterAutoMode)(struct tdc_struct_t *);
    void (*Link)(struct tdc_struct_t *, void * );
    void (*Next)(struct tdc_struct_t *);
}tdc_handle_t;


/* ===============LACO=================*/
// 1 - LACO  Constant Definition
// 2 - LACO config structure
// 3 - Laco handler
typedef struct lacos_struct_t{
    void * ptr_internal;
    //lacos_caps_val: initial value of caps
    // X X X X X X X X
    // - -            :bit[6..7] reserved
    // X = 1 in bit[N], N cap is seleted.
    // for example:
    // 0 0 1 1 0 0 0 0 will select cap4..5
    uint8_t lacos_caps_val[4];
    //length of pwm waveform (pulse length)
    // for lacoX. (X = 1~4)
    uint16_t lacos_pwm_len[4];
    
    void (*Init)(struct lacos_struct_t *);
    void (*Next)(struct lacos_struct_t *);
    void (*SetCaps)(struct lacos_struct_t *, uint8_t,uint8_t);
    void (*Start)(struct lacos_struct_t *);
    void (*Link)(struct lacos_struct_t *, pwm_handle_t *);
}lacos_handle_t;

/* ===============LEDS=================*/
//This variable is used to save leds states
// X  XXXX  XXXX
// 1  ----  ----: System state
// -  XXXX  ----: Laco state: ON when vehicle detected.
// 4->|  |<- Laco 1
// -  ----  XXXX: Laco Error indicator: ON error occored
typedef struct leds_struct_t{
	void * lock;
	uint16_t leds_state;

	void (*Init)(struct leds_struct_t * handle);
	void (*Update)(struct leds_struct_t * handle);
	void (*Set)(struct leds_struct_t * handle, uint8_t bit_loc);
	void (*Clr)(struct leds_struct_t * handle, uint8_t bit_loc);
}leds_handle_t;

/* ===============I2C=================*/
// 1 - I2C config structure
typedef struct{
    uint8_t id;
    //Slave address, only 7bit address supported
    uint8_t addr;
    //speed = 10kHz - 400kHz
    uint32_t speed;

} i2c_conf_t;

typedef struct i2c_struct_t{
    //id of I2C module:
    // 1 = I2C1
    // 0 = I2C0
    uint8_t id;
    // Current Slave address
    uint8_t addr;
    //internal memory area
    void * ptr_internal;
    //Init function
    void (*Init)(struct i2c_struct_t *, i2c_conf_t *);
    void (*Read)(struct i2c_struct_t *, uint8_t *,uint8_t);
} i2c_handle_t;

/* ============WatchDog================*/
typedef struct watchdog_struct_t {
	//watchdog mark
	// Any thread or task needs to write to mark
	// every 500ms. Written by all the threads
	// except the watchdog thread.
	volatile uint8_t mark;
	// The code to write, update every 1 seg.
	// Written by watchdog thread
	volatile uint8_t code;
	// cnter of missed feed dog for arm watchdog
	// structure.
	// reserved for DSP watchdog structure.
	uint8_t cnter;
	//pointer of DSP mark, if this is a watch-
	//dog structure for ARM, the value should
	// be the address of watchdog structure for
	// DSP, else it should be null.
	struct watchdog_struct_t * ptr_wdog_dsp;
	void (*Init)(struct watchdog_struct_t *);
	void (*GiveFood)(struct watchdog_struct_t *);
#ifndef OMAPL138_DSP
	void (*Feed)(struct watchdog_struct_t *);
#endif
} watchdog_handle_t;


/* ================ADC (Copy from TDC)===============*/
typedef struct {
    uint8_t dev_id;
    //Communication handle
    //void * comm_handle;
    //channel chain configuration
} adc_conf_t;
typedef struct tdc_struct_t adc_handle_t;

/* ================Dipswitch ====================*/
typedef struct ds_struct_t{
//	uint32_t ds_val; //current dipswitch value
	uint8_t has_changed;//write 1 if value is changed
	void * ds_internal;
	void (*Init)(struct ds_struct_t *);
	void (*Update)(struct ds_struct_t *);
	uint32_t (*Read)(struct ds_struct_t *);
} ds_handle_t;


/* ===============ERROR==========================*/
typedef struct eb_struct_t{
	//Event = 0, ERROR = 1
	uint8_t type;
	//Event id or Error id
	uint8_t id;
	//Extra data if has.
	void * data;
}eb_t;


/*=========================================
 * External variables declaration
 *=========================================*/
//=================Board Structure==================*/
struct board_struct_t{
	struct board_info_t{
	    uint32_t board_no;
	} info;
    //1 - adc handle: AD7293
	struct tdc_struct_t * adc1;
    //2 - adc handle: MCP3021
	struct tdc_struct_t * adc2;
    //3 - tdc handle: TDC7201_1_CH1
	struct tdc_struct_t * tdc;
    //7 - dipswitch:
    struct ds_struct_t * dipswitch;
    //8 - laco: laco control
	struct lacos_struct_t * lacos;
    //9 - leds
	struct leds_struct_t * leds;
    //10 - uart0
	struct uart_struct_t * uart0;
    //11 - uart1
	struct uart_struct_t * uart1;
    //12 - watchdog
	struct watchdog_struct_t * watchdog;
    //13 - error handle
    //err_handle_t * error;
};
typedef struct board_struct_t board_handle_t;

//Function prototype
void ConnectorInit(board_handle_t *board);

#endif /* BSP_DRIVERS_CONNECTOR_H_ */
