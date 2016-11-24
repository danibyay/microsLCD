/*
 * LCDManager.c
 *
 *  Created on: Nov 18, 2016
 *      Author: Daniela Becerra, Jesus Martin
 */


#include "LCDManager.h"
#include "types.h"
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */


const u8 InitLCD[] = {0x39, 0x39, 0x15, 0x55, 0x6E, 0x72, 0x38, 0x0F, 0x01, 0x06};
const u8 msgSize = 24;
const u8 initInstructions = 10;
u8 instCounter = 0;
u8 msgCounter = 0;
bool newMsg = M_TRUE;
bool initmode;
bool sending;
u8 LCDDispData[] = {" CW  - 20.0 C    0.2 RPS"};

/* private function definitions *************************************/
interrupt VectorNumber_Vrti void ISR_RTI (void);
interrupt VectorNumber_Vtpmch1 void ISR_TPMC1(void);


/* functions implementation ***************************************/
/*****************************************************************************************/ 
void LCDManager_setData(u8 DispDataParam[])
{
	u8 i;
    for(i=0; i<msgSize; i++)
    {
    	LCDDispData[i] = DispDataParam[i];
    }
    LCDDispData[0] = DISPLAY_CLEAR;
}

/*****************************************************************************************/ 

void LCDManager_Init(void)
{
  LCDDispData[0] = DISPLAY_CLEAR;

  SPIC1 = SPICONF1;  
  SPIC2 = SPICONF2;  
  SPIBR = SPICONF3;   

  RS_PINDIR = OUTPUT; 
  RS_PIN = RS_COMMAND;
  initmode = M_TRUE;
  sending = M_TRUE;

  TPMSC = TPM_CONF; 	  /* Configure TPM */ 
  TPMC1SC = CH1_CONF;   /* Initialize TPM Channel 0 */
  TPMC1V = DELAY_LCD;  /* Initialize output-compare value */	
  SRTISC = RTI_CONF;   /* Configure RTI for 1 second interruptions */
}

/*****************************************************************************************/ 
interrupt VectorNumber_Vtpmch1 void ISR_TPMC1(void) 
{	
	/*TPM_ISR ****************************/
	(void)TPMSC;	
	TPMC1SC_CH1F = FLAG_OFF;    
	TPMC1V += DELAY_LCD;
	/*Send Data **********************/
	if (SPIS_SPTEF)
		{
			if(initmode)
			{
				if(instCounter < initInstructions)
				{
					SPID = InitLCD[instCounter++];

				}
				else
				{
					initmode = M_FALSE;
					sending = M_FALSE;
					LCDMANAGER_STOP;
				}
			}
			else
			{
				if (msgCounter < msgSize)
				{
					if(msgCounter == 1)
					{
						RS_PIN = RS_DATA;	
					}
					SPID = LCDDispData[msgCounter++];
				}
				else
				{
					msgCounter = 0;
					sending = M_FALSE;
					LCDMANAGER_STOP;
				}
			}
		}	

	
}
/*****************************************************************************************/ 
interrupt VectorNumber_Vrti void ISR_RTI (void)
{
	LCDMANAGER_RTI_ISR;
	/* LCDManager_Refresh **************/
	if (sending == M_FALSE)
	{
		RS_PIN = RS_COMMAND;
		LCDMANAGER_START;
		sending = M_TRUE;
	}
}

