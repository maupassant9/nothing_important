/*
 * AdmComm.h
 *
 *  Created on: 15 de fev de 2018
 *      Author: dong.x
 */

#ifndef APP_ADMCOMM_ADMCOMM_H_
#define APP_ADMCOMM_ADMCOMM_H_


  typedef struct queue_cmd_ele_t {
      Queue_Elem _elem;
      dyn_array_t * data;
  } queue_cmd_ele_t;


#endif /* APP_ADMCOMM_ADMCOMM_H_ */
