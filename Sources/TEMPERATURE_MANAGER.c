/*! **************************
*
* @file: ADC_Manager.c
* @author(s): Daniela Becerra, Jesús Barroso
*
* @brief
*Includes two basic functions of the ADC
*
************************** */

/*********************						Includes						**********************/
#include "TEMPERATURE_MANAGER.h"
#include "ADCMANAGER.h"
#include "types.h"
#include "derivative.h" /* include peripheral declarations */

/*********************				  Defines								**********************/
/*********************				   Exported Functions					******************x	****/
u16 TEMPERATURE_Read()
{
	u16 temp;
	temp= (u16)((u32)ADC_Read()*13);
	return 	temp;

}

void TEMPERATURE_Init()
{
	ADC_Init();
}

bool Temperature_isDataReady()
{
	return ADC_isDataReady();
}
//-------------------------------------------------------------------------------------------------
