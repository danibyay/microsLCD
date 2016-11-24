/*
 * LCDManager.h
 *
 *  Created on: Nov 18, 2016
 *      Author: Danibw
 */

#ifndef LCDMANAGER_H_
#define LCDMANAGER_H_

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "types.h"

#define TPM_CONF 		0b00001010 /* No overflow interrupt, BUSCLK, Prescaler 4 (1us pulses) */
#define CH1_CONF 		0b01010000 /* Interrupt enabled, output-compare, software compare only */  
#define FLAG_OFF 		0
#define DELAY_LCD  		1220 // (5 milliseconds)
#define DISPLAY_CLEAR 0x01
#define RTI_CONF 0b00010111  // 1 segundo
#define OUTPUT 1
#define RS_DATA 1
#define	RS_COMMAND 0
#define RS_PINDIR PTBDD_PTBDD4		/* GPIO Pin direction register */
#define RS_PIN PTBD_PTBD4		/* GPIO Pin data register */



void LCDManager_Init(void);

void LCDManager_setData(u8 DispDataParam[]);

#endif /* LCDMANAGER_H_ */
