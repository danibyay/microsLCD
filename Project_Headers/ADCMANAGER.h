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
#ifndef ADC
#define ADC
/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "types.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define ADC_ANALOGIC_INPUT  1
#define REG_CONFIG 0b00100000 //no interruptions, continous conversion and channel zero

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
/*!*-----------------------------------------------------------------------------------------------
* \name     ADC_Init
* \brief    Initalizes the ADC module used for the ADC manager.
* \param    void
* \return   void
-------------------------------------------------------------------------------------------------*/
void ADC_Init(void);

/*!*-----------------------------------------------------------------------------------------------
* \name     ADC_Read
* \brief    Reads ADCRL register when COCO FLAG is on
* \param    void
* \return   void
-------------------------------------------------------------------------------------------------*/
u8 ADC_Read(void);

/*!*-----------------------------------------------------------------------------------------------
* \name     ADC_isDataReady
* \brief    reads COCO FLAG to determine if the data conversion is complete
* \param    void
* \return   bool, true or false
-------------------------------------------------------------------------------------------------*/
bool ADC_isDataReady(void);

#endif /* ADC */
