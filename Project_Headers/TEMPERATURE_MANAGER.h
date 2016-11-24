/* ! **************
* @file: ADCManager.h
* @author(s): Daniela Becerra, Jes�s Barroso
*
* @brief (User's Manual)
* This manager controls how we are using the ADC Module(Register, Operation mode, Number of Channel)
*
* You can change between channels
*
***************** */

#ifndef TEMPERATURE_MANAGER_H_
#define TEMPERATURE_MANAGER_H_
/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "types.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/

#define MAX_SENSOR_VOLTAGE 3300
#define MAX_ADC_READ 255
#define SENSOR_OFFSET 20
/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
void TEMPERATURE_Init(void);
/*!*-----------------------------------------------------------------------------------------------
* \name     Temperature_Init
* \brief    Initalizes the ADC module used for the ADC manager.
* \param    void
* \return   void
-------------------------------------------------------------------------------------------------*/
u16 TEMPERATURE_Read(void);
/*!*-----------------------------------------------------------------------------------------------
* \name     Temperature_Read
* \brief    Reads ADCRL register when COCO FLAG is on
* \param    void
* \return   void
-------------------------------------------------------------------------------------------------*/

bool Temperature_isDataReady(void);
/*!*-----------------------------------------------------------------------------------------------
* \name     Temperature_isDataReady
* \brief    if the ADC data is ready, returns true, or else, false.
* \param    void
* \return   bool
-------------------------------------------------------------------------------------------------*/

#endif /* ADC */
