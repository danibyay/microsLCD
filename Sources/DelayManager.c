/* ! **************
* @file: DelayManager.c
* @author(s): Daniela Becerra, Jesús Barroso
*
*
***************** */
//Includes a function to initialize a timer, and a function to calculate delays in microseconds
/*********************						Includes						**********************/
#include "GPIO.h"
#include "DelayManager.h"
#include "DirectionManager.h"
#include "types.h"
#include "TimersManager.h"


/*********************						Variables							**********************/

/*********************				   Exported Functions					**********************/
u8 DelayManager_Init(u16 time)
{
	u8 id = TimersMngr_CreateTimer(M_NULL);
	TimersMngr_StartTimer(id, time);
	return id;
}
//-------------------------------------------------------------------------------------------------
u16 DelayManager_getTimeUs(u16 rps)
{
	u16 time;
	u16 a;
	a=((u32)STEPS * rps);
	a=(u16) (SCALE/a);
	if(a > MIN_SPEED)
	{
		return MIN_SPEED;
	}
	else if(a < MAX_SPEED)
	{
		return MAX_SPEED;
	}
	return a;
}
