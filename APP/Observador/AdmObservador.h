/*
 * AdmObservador.h
 *
 *  Created on: 9 de fev de 2018
 *      Author: dong.x
 */

#ifndef APP_OBSERVADOR_ADMOBSERVADOR_H_
#define APP_OBSERVADOR_ADMOBSERVADOR_H_

//the period of Observador, in ms
//should be the same with the configuration
// in app.cfg.
#define OBSERVADOR_PERIOD 5

//The period of every check.
#define WATCHDOG_CHECK_PERIOD 800
#define LEDS_CHECK_PERIOD 10
#define DIPSWITCH_CHECK_PERIOD 1000

//The maximum value of counter
#define CNTER_MAX_VAL 1000

#define WATCHDOG_THRESHOLD_VAL WATCHDOG_CHECK_PERIOD/OBSERVADOR_PERIOD
#define LEDS_THRESHOLD_VAL LEDS_CHECK_PERIOD/OBSERVADOR_PERIOD
#define DIPSWITCH_THRESHOLD_VAL DIPSWITCH_CHECK_PERIOD/OBSERVADOR_PERIOD

#endif /* APP_OBSERVADOR_ADMOBSERVADOR_H_ */
