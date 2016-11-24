/*! **************************
*
* @file:MessageMAnager.c
* @author(s): Daniela Becerra, Jesús Barroso
*
* @brief
* The message Manager is used to correctly send and store messages from a UART using two buffers.
*
************************** */

/*********************						Includes						**********************/
#include "MessageManager.h"
#include "SCI.h"
#include "types.h"
#include "LCDManager.h"

static u8 MsgManager_Buffer1[MAX_MSG_SIZE];	
static u8 MsgManager_Buffer2[MAX_MSG_SIZE];	
static bool Buffer_Select;
static bool MsgManagerBuffer1_isFull;			//Two buffers in case of very fast incoming messages
static bool MsgManagerBuffer2_isFull;
static u8 RxIndex;

u8 DispData[] = {" CW  - 20.1 C    0.2 RPS"}; 

void MsgManager_Init()
{
	SCI_InitTx();
	SCI_InitRx();
	LCDManager_Init();
	SCI_SetRxCallBack(&MsgManager_ReceiveMessage);
	
	RxIndex = ZERO;
	Buffer_Select= ZERO;
	MsgManagerBuffer1_isFull = M_FALSE;
	MsgManagerBuffer2_isFull = M_FALSE;
	
}



void MsgManager_updateDir(bool dirCW)
{
	u8 index = DIR_INDEX;
    if(!dirCW)
    {
    	DispData[index] = C_LETTER;
    	index++;
    	DispData[index] = W_LETTER;
    }
    else
    {
    	DispData[index] = W_LETTER;
    	index++;
    	DispData[index] = SPACE;
    }
    LCDManager_setData(DispData);
}


void MsgManager_updateSpeed(u8 *newSpeed)
{
	u8 index = RPS_INDEX;
	DispData[index] = *newSpeed;
	newSpeed+=2;  
	index+=2;
	DispData[index] = *newSpeed;
	
	LCDManager_setData(DispData);
}

void MsgManager_updateTemp(u8 * newTemp)
{
	u8 index = TEMP_INDEX;
	DispData[index] = *newTemp;
	index++; 
	newTemp++;
	DispData[index] = *newTemp;
	index+=2;  //double advance because the decimal point wont change
	newTemp+=2;
	DispData[index] = *newTemp;
	
	LCDManager_setData(DispData);
	
}

void MsgManager_SendMessage(const u8 message[], u8 size)
{
	SCI_SendMessage(message,size);
}


bool MsgManager_TxisBusy()
{
	return SCI_TxIsBusy();
}


void MsgManager_GetMessageReceived(char msg[])   // it receives a pointer with the direction of a variable outside msgManager that is about to receive the message
{
	u8 bufferIndex;
	bufferIndex = 0;
	if(MsgManagerBuffer1_isFull)
	{
		while(MsgManager_Buffer1[bufferIndex] != 0)		
		{
			msg[bufferIndex] = MsgManager_Buffer1[bufferIndex];
			MsgManager_Buffer1[bufferIndex] = 0;			//Erase every index copied
			bufferIndex++;	
		}
	MsgManagerBuffer1_isFull = 0;								//the buffer is now empty
	}
	else
	{
		while(MsgManager_Buffer2[bufferIndex] != 0)		
		{
			msg[bufferIndex] = MsgManager_Buffer2[bufferIndex];
			MsgManager_Buffer2[bufferIndex] = 0;
			bufferIndex++;	
		}
		MsgManagerBuffer2_isFull = 0;
	}
}
 


bool MsgManager_isNewMessageReady (void)
{
	return (MsgManagerBuffer1_isFull || MsgManagerBuffer2_isFull);		//true if any of the buffers is full
}

void MsgManager_clearMsg(u8 msg[])
{
	u8 index=0;
	for(index;index<MAX_MSG_SIZE;index++)
	{
		msg[index]=0;
	}
}

void MsgManager_ReceiveMessage(void)			
{
	if((SCI_getChar() == CARRIAGERETURN) || (RxIndex >= MAX_RX_MSG_SIZE)) 	//If the character received is a carriage return	
	{	
		Buffer_Select = (Buffer_Select ^ 1);	//change the buffer for next character received
		if(Buffer_Select == BUFFER2) 			//Selector pointing to second buffer, now Buffer 1 is full
		{
			MsgManagerBuffer1_isFull = 1;							             
		}
		else
		{
			MsgManagerBuffer2_isFull = 1;			
										
		}
		RxIndex = 0;
	}
	else if (Buffer_Select == BUFFER1)			// start writing in the buffer 1
	{
		MsgManager_Buffer1[RxIndex] = SCI_getChar();	
		RxIndex ++;
	}
	else if (Buffer_Select == BUFFER2)
	{
		MsgManager_Buffer2[RxIndex] = SCI_getChar()	;	
		RxIndex ++;
	}
}
