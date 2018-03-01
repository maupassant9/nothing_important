
/*
* FileName: dipswitch.h
* Author: Dong Xia
* This is head file of dipswitch
*
* Change Records:
*      >> (22/Fev/2018): Creation of file
*
*/

#ifndef BSP_DRIVERS_DIPSWITCH_DIPSWITCH_H_
#define BSP_DRIVERS_DIPSWITCH_DIPSWITCH_H_

/********************************************
* Include
********************************************/


/********************************************
* Macro
********************************************/

/********************************************
* Type definition
********************************************/


/********************************************
* Function prototype
********************************************/
/*============================================================
 * Function:  DrvDsInit
 * Description:  Initiate the dip switch and data structure
 * Para:
 *    >>  :  an adc handle.
 *    >>  :  an adc configuration structure.
 * Return:
 *     >>
 * Change Record:
 *		>> (22/Fev/2018): Creation of the function;
 *
 *============================================================*/
 void DrvDsInit();

 /*============================================================
  * Function:  DrvDsUpdate()
  * Description:  Update the value of DIPSwitch, if the value
  * changes, update the has_changed bit.
  * Para:
  *    >>  :
  * Return:
  *     >>
  * Change Record:
  *		>> (22/Fev/2018): Creation of the function;
  *
  *============================================================*/
 void DrvDsUpdate(ds_handle_t * handle);

 /*============================================================
  * Function:  DrvDsRead()
  * Description:  Read the switch value
  * Para:
  *    >> ds_handle_t * : dip switch data structure.
  * Return:
  *     >> uint32_t: switch value:
  *     |       |<-Reserved           |_______|<-DIP2
  *     XXXX XXXX XXXX XXXX XXXX XXXX XXXX XXXX
  *         DIP4->|_______| |_______|<-DIP3
  * Change Record:
  *		>> (22/Fev/2018): Creation of the function;
  *
  *============================================================*/
 uint32_t DrvDsRead(ds_handle_t * handle);

#endif /* BSP_DRIVERS_DIPSWITCH_DIPSWITCH_H_*/
