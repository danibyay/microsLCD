/*
 * LCDManager.h
 *
 *  Created on: Nov 18, 2016
 *      Author: Daniela Becerra, Jesus Martin
 */

#ifndef LCDMANAGER_H_
#define LCDMANAGER_H_

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "types.h"

#define TPM_CONF 		0b00001010 /* No overflow interrupt, BUSCLK, Prescaler 4 (1us pulses) */
#define CH1_CONF 		0b01010000 /* Interrupt enabled, output-compare, software compare only */  
#define FLAG_OFF 		0
#define DELAY_LCD  		1220 /* (1.2 milliseconds) */
#define DISPLAY_CLEAR 0x01
#define RTI_CONF 0b00010111  /* 1 second */
#define OUTPUT 1
#define RS_DATA 1
#define	RS_COMMAND 0
#define RS_PINDIR PTBDD_PTBDD4		/* GPIO Pin direction register */
#define RS_PIN PTBD_PTBD4		/* GPIO Pin data register */
#define LCDMANAGER_STOP 	TPMC1SC_CH1IE = M_FALSE
#define LCDMANAGER_START    TPMC1SC_CH1IE = M_TRUE
#define LCDMANAGER_RTI_ISR  SRTISC_RTIACK = M_TRUE
#define SPICONF1  0b01010010
#define SPICONF2  0b00011001
#define SPICONF3  0x0F

/* Function prototypes ***************************************/

/*!*-----------------------------------------------------------------------------------------------
* \name     LCDManager_Init
* \brief    Initializes the Registers for SPI communication and necessary timers
* \param    void
* \return   void
-------------------------------------------------------------------------------------------------*/
void LCDManager_Init(void);


/*!*-----------------------------------------------------------------------------------------------
* \name     LCDManager_setData
* \brief    Used by an upper layer to modify the character array that is displayed on the LCD
* \param    u8 array DispDataParam
* \return   void
-------------------------------------------------------------------------------------------------*/
void LCDManager_setData(u8 DispDataParam[]);

#endif /* LCDMANAGER_H_ */
