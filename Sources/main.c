/*! **************************
*
* @file: Main.c
* @author(s): Daniela Becerra, Jesús Barroso
*
* @brief
*  Main program of an application that controls a stepper-motor via serial protocol
*  Use an emulator terminal, like tera-term. Enable local echo and set new line characters CR
*  Use the commands described in the message constants to move, turn on/off, change speed, etc. of the motor.
*
************************** */

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "MotorManager.h"
#include "TEMPERATURE_MANAGER.h"
#include "DelayManager.h"
#include "DirectionManager.h"
#include "TimersManager.h"
#include "types.h"
#include "StringUtilis.h"
#include "MessageManager.h"


#define DEGREES_HALF_STEP 		375
#define DEFAULT_STEPPER_TIME 52000
#define DEFAULT_TEMP_TIME 5000000
#define MAX_TEMP_DEFAULT 	400 //40 degrees
#define MAX_MSG_SIZE  26//tal vez mas despues


const u8 msgDistanceSize = 5;
u8 tempTimerID;	

u16 variableStepperTime;
bool motorDirection;
char currMsg[MAX_MSG_SIZE];
bool motorState;
bool msgSuccess;
u16 temperatureLimit;
u8 tempMsg[] = "\n   . C\n\r";
const bool clockWise = 1;
const bool counterClockWise = 0;
const u8 msgTempLim[]="TEMP-LIMIT:";
const u8 msgTempLimSize=11;
const u8 optTempLimSize=5;
const u8 msgTempSize=9;
const u8 numAfterDec=1;
const u8 optRpsSize = 5;
const u8 optDegreeSize = 3;
const u8 optDirCw[] = "CW\r";
const u8 optDirCcw[] = "CCW\r";
const u8 optDirCwSize = 2;
const u8 optDirCcwSize = 3;
const u8 optMotorOff[] = "Off\r";
const u8 optMotorOn[] = "On\r";
const u8 optMotorOnSize = 2;
const u8 optMotorOffSize = 3;
const u8 msgMotorSize = 6;
const u8 msgRpsSize = 4;
const u8 msgDirSize = 4;
const u8 msgStepSize = 8;
const u8 msgStepSizeCcw = 9;
const u8 msgProfSaveSize = 9;
const u8 msgProfUseSize = 8;
const u8 msgDoneSize = 7;
const u8 invalidMsgSize = 12;
const u8 msgMotor[] = "MOTOR:";
const u8 msgRps[] = "RPS:";
const u8 msgDir[] = "DIR:";
const u8 msgStep[] = "STEP-CW:";
const u8 msgStepCcw[] = "STEP-CCW:";
const u8 msgProfSave[] = "PROF_SAVE:"; //:n
const u8 msgProfUse[] = "PROF_USE:"; //:n
const u8 msgDone[] = "\nDone\n\r";
const u8 invalidMsg[] = "\nTry again\n\r";
const u8 overTempDetect[]="OverTempeture Detected\n\r";
const u8 overTempDetectsize=24;
const u8 msgDistance[] = "DIST:";
const u8 optDistanceSize = 4;
u16 tempDg;
const u8 twoNumAfterDec = 2;
const u8 numAfterDec = 1;

void tryAgain(void)
{
	if (!MsgManager_TxisBusy())
	{
		MsgManager_SendMessage(invalidMsg, invalidMsgSize);
		msgSuccess = M_FALSE;
	}
}

void main(void) 
{
	u16 systemTemp;
	u8 motorTimerID;
	EnableInterrupts;
	
	TimersMngr_Init(); 
	motorTimerID = DelayManager_Init(DEFAULT_STEPPER_TIME);	
	tempTimerID = DelayManager_Init(DEFAULT_TEMP_TIME);
	DirectionManager_Init(); 
	MotorStringManager_Init();
	TEMPERATURE_Init(); 
	MsgManager_Init();
	
	
	variableStepperTime = DEFAULT_STEPPER_TIME;
	motorDirection = clockWise;
	motorState = ON;
	msgSuccess = M_FALSE;
	temperatureLimit=MAX_TEMP_DEFAULT;

	
	for(;;) {
	    TimersMngr_PeriodicTask();
	    TimersMngr_UpdateCount();
	    
	    if(Temperature_isDataReady())
	    {
		    systemTemp=TEMPERATURE_Read();
	    }
	    // MOTOR
	    if(TimersMngr_TimerHasElapsed(motorTimerID))
	        {
	    			if(motorState == ON & systemTemp<temperatureLimit)
	    			{
		    			DirectionManager_Move();
	    			}
	    	    	TimersMngr_StartTimer(motorTimerID, variableStepperTime);
	    	    	DirectionManager_setDirection(motorDirection);	
	    	}
	    
	    //RECEIVE MESSAGE
	    if(MsgManager_isNewMessageReady())
	    {
	    	MsgManager_GetMessageReceived(currMsg);
	    	
	    	//poner la referencia como const char o const u8 para que no haya warning en los
	    	//prototipos de funciones

	    	
	    	if(StringUtils_strcmp(currMsg, msgMotor, msgMotorSize))
	    	{
	    		if(StringUtils_strcmp(&currMsg[msgMotorSize], optMotorOn, optMotorOnSize))
	    		{
	    			motorState = ON;
	    			msgSuccess = M_TRUE;
	    		}
	    		else if(StringUtils_strcmp(&currMsg[msgMotorSize], optMotorOff, optMotorOffSize))
	    		{
	    			motorState = OFF;
	    			msgSuccess = M_TRUE;
	    		}
	    		else
	    		{
	    			tryAgain();
	    		}
	    	}
	    	//RPS COMMAND
	    	else if(StringUtils_strcmp(currMsg, msgRps, msgRpsSize))
		    {
		    	u16 tempRps;
		    	MsgManager_updateSpeed(&currMsg[msgRpsSize+2]);
		    	if(StringUtils_Str2Num(&currMsg[msgRpsSize], optRpsSize, numAfterDec, &tempRps))
		    	{
					variableStepperTime = DelayManager_getTimeUs(tempRps); 
					msgSuccess = M_TRUE;
		    	}
		    	else
		    	{
		    		tryAgain();
		    	}
		    }
	    	//DIRECTION COMMAND
	    	else if(StringUtils_strcmp(currMsg, msgDir, msgDirSize))
	    	{
	    		if(StringUtils_strcmp(&currMsg[msgDirSize], optDirCw, optDirCwSize))
	    		{
	    			motorDirection = clockWise;
	    			msgSuccess = M_TRUE;
	    		}
	    		else if(StringUtils_strcmp(&currMsg[msgDirSize], optDirCcw, optDirCcwSize))
	    		{
	    			motorDirection = counterClockWise;
	    			msgSuccess = M_TRUE;
	    		}
	    		else
	    		{
	    			tryAgain();
	    		}
	    		MsgManager_updateDir(motorDirection);
	    	}
	    	//Temperature Limit command
	    	else if(StringUtils_strcmp(currMsg, msgTempLim, msgTempLimSize))
	    	{
	       		u16 tempTempLim;
	      		if(StringUtils_Str2Num(&currMsg[msgTempLimSize], optTempLimSize, numAfterDec, &tempTempLim))
	       		{
	       			temperatureLimit=tempTempLim;
	       			msgSuccess = M_TRUE;   			
	    		}
	    		else
	    		{
	    		  	tryAgain();
	    		}
	        }
	    	else if(motorState==OFF)
	    	{
	    		//DEGREES COMMAND CLOCK WISE
	    		if(StringUtils_strcmp(currMsg, msgStep, msgStepSize))
	    		{
	    			u16 tempDeg;
	    			u32 degrees;
	    			if(StringUtils_Str2Num(&currMsg[msgStepSize], optDegreeSize, 0, &tempDeg))
	    			{
		    			degrees = (u32)tempDeg*100 ;
		    			TimersMngr_StartTimer(motorTimerID, variableStepperTime);
		    			while(degrees > DEGREES_HALF_STEP) 
		    			{
		    				__RESET_WATCHDOG();
	    				    TimersMngr_PeriodicTask();
	    				    TimersMngr_UpdateCount();
		    				if(TimersMngr_TimerHasElapsed(motorTimerID))
		    				{	    		
		    					TimersMngr_StartTimer(motorTimerID, variableStepperTime);
		    					DirectionManager_Move();
		    	    	    	DirectionManager_setDirection(clockWise);	
		    					degrees =degrees-DEGREES_HALF_STEP;
		    				}
	    					
		    			}
		    			MsgManager_clearMsg(&degrees); // alomejor no es necesario
		    			msgSuccess = M_TRUE;
	    			}
	    			else
	    			{
	    				tryAgain();
	    			}

	    		}
	    		//DEGREES COMMAND COUNTER CLOCK WISE
	    		else if(StringUtils_strcmp(currMsg, msgStepCcw, msgStepSizeCcw))
	    		{
	    			u16 tempDeg;
	    			u32 degrees;
	    			if(StringUtils_Str2Num(&currMsg[msgStepSizeCcw], optDegreeSize, 0, &tempDeg))
	    			{
	    				degrees = (u32)tempDeg*100 ;
	    			
	    					   TimersMngr_StartTimer(motorTimerID, variableStepperTime);
	    					    while(degrees > DEGREES_HALF_STEP)
	    					    {
	    					    					__RESET_WATCHDOG();
	    					    					TimersMngr_PeriodicTask();
	    					    					TimersMngr_UpdateCount();
	    					    					if(TimersMngr_TimerHasElapsed(motorTimerID))
	    					    					{	    
	    					    						TimersMngr_StartTimer(motorTimerID, variableStepperTime);
	    					    						DirectionManager_Move();
	    					    						DirectionManager_setDirection(counterClockWise);	
	    					    						degrees =degrees-DEGREES_HALF_STEP;
	    					    					}
	    					    				    					
	    					    				}
	    					    				MsgManager_clearMsg(&degrees); // alomejor no es necesario
	    					    				msgSuccess = M_TRUE;

	    			}
	    			else
	    			{
	    				tryAgain();
	    			}
	    		}	
	    		else
	    		{
	    			tryAgain();
	    		}
	    	}
	    	else
	    	{
	    		tryAgain();
	    	}

	    }
	    
	    //ACKNOWLEDGE MESSAGE
	    if (msgSuccess)
	    {
	    		if (!MsgManager_TxisBusy())
	    		{
	    			MsgManager_SendMessage(msgDone, msgDoneSize);
	    			msgSuccess = M_FALSE;
	    			MsgManager_clearMsg(&currMsg);
	    		}
	    }
	    
	    //TEMPERATURE
	    
	    if (TimersMngr_TimerHasElapsed(tempTimerID))
	    {
	    	
		    if(!MsgManager_TxisBusy())
		    {
		    		__RESET_WATCHDOG();
		    		StringUtils_Num2Str(systemTemp, tempMsg);
		    		MsgManager_updateTemp(&tempMsg[1]);
		    		MsgManager_SendMessage(tempMsg, msgTempSize); 
		    		TimersMngr_StartTimer(tempTimerID, DEFAULT_TEMP_TIME);
		    }

	    }
	    
	    
	    		
	    
		__RESET_WATCHDOG();	/* feeds the dog */
	} /* loop forever */
}


