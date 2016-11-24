/*
 * MessageManager.h
 *
 *  Created on: Oct 29, 2016
 *      Author: Jesus Martin, Daniela Becerra
 *      HIL layer above serial protocol module
 */
#include"types.h"
#ifndef MESSAGEMANAGER_H_

#define MESSAGEMANAGER_H_
#define CARRIAGERETURN 13
#define MAX_RX_MSG_SIZE 26
#define BUFFER1 0
#define BUFFER2 1
#define ZERO    0

#define DIR_INDEX   2   
#define RPS_INDEX   17 
#define TEMP_INDEX  7  
#define RPS_SIZE 3
#define TEMP_SIZE 4
#define SPACE 0x20
#define W_LETTER 0x57
#define C_LETTER 0x43

/*!*-----------------------------------------------------------------------------------------------
* \name     MsgManager_SendMessage
* \brief    Returns degrees to turn given a speed with a previously set time
* \param    distance, the distance that the motor will make a tire advance
* \return   degrees, the necessary degrees to move the motor such distance
-------------------------------------------------------------------------------------------------*/
void MsgManager_SendMessage(const u8 message[], u8 size);

/*!*-----------------------------------------------------------------------------------------------
* \name     MsgManager_Init
* \brief    Initializes Serial modules and default values for messageManager
* \param    void
* \return   void
-------------------------------------------------------------------------------------------------*/
void MsgManager_Init(void);

/*!*-----------------------------------------------------------------------------------------------
* \name     MsgManager_ReceiveMessage
* \brief    Saves  the data in the Serial Register to the buffers, and switchs between them for speed and availability
* \param    void
* \return   void
-------------------------------------------------------------------------------------------------*/
void MsgManager_ReceiveMessage(void);

/*!*-----------------------------------------------------------------------------------------------
* \name     MsgManager_TxisBusy
* \brief    Serves to know if we're able to send something.
* \param    void
* \return   bool, true is the transmitter is busy  
-------------------------------------------------------------------------------------------------*/
bool MsgManager_TxisBusy(void);

/*!*-----------------------------------------------------------------------------------------------
* \name     MsgManager_GetMessageReceived
* \brief    Returns into a string of characters the message received by SCI
* \param    array of chars, where the received message will be saved
* \return   void
-------------------------------------------------------------------------------------------------*/
void MsgManager_GetMessageReceived(char msg[]);

/*!*-----------------------------------------------------------------------------------------------
* \name     MsgManager_isNewMessageReady
* \brief    Returns a boolean to say if the message in the buffer is ready
* \param    void
* \return   bool, True if message is ready
-------------------------------------------------------------------------------------------------*/
bool MsgManager_isNewMessageReady (void);

void MsgManager_updateSpeed(u8 *newSpeed);

void MsgManager_updateDir(bool dirCW);

void MsgManager_updateTemp(u8 * newTemp);

/*!*-----------------------------------------------------------------------------------------------
* \name     MsgManager_clearMsg
* \brief    Clears the array of any previous characters saved.
* \param    u8 array, message to be cleared
* \return   void
-------------------------------------------------------------------------------------------------*/
void MsgManager_clearMsg(u8 msg[]);

#endif /* MESSAGEMANAGER_H_ */
