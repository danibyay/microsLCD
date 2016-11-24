/*! **************************
*
* @file: Motor_String_Manager.c
* @author(s): Daniela Becerra, Jesús Barroso
*
* @brief
* 8 sequences of 8 bits, with the 4 least significant are the ones that
* determine the movement of a step motor with a half step sequences
* These 8 sequences are located in an array, and by traversing it
* in one way or another, the motor will turn on.
*
************************** */

/*********************						Includes						**********************/
#include "GPIO.h"
#include "MotorManager.h"

/*********************						Defines							**********************/

#define BITS 8
/*********************						Typedefs						**********************/

/*********************					Function Prototypes					**********************/

/*********************                  Static Variables                    **********************/
static u8 portA2seq[] = {0,0,0,0,0,1,1,1};
static u8 portA3seq[] = {0,0,0,1,1,1,0,0};
static u8 portB6seq[] = {0,1,1,1,0,0,0,0};
static u8 portB7seq[] = {1,1,0,0,0,0,0,1};
/*************************************************************************************************/
u8 s_index = 0; //sequence index
/*********************                  Static Constants                    **********************/
/*********************                  Global Constants                    **********************/
/*********************				   Exported Functions					**********************/

void MotorStringManager_Init(void)
{
	PTADD_PTADD2 = 1;
	PTADD_PTADD3 = 1;
	PTBDD_PTBDD6 = 1;
	PTBDD_PTBDD7 = 1;
	PTAD_PTAD2 = portA2seq[s_index];
	PTAD_PTAD3 = portA3seq[s_index];
	PTBD_PTBD6 = portB6seq[s_index];
	PTBD_PTBD7 = portB7seq[s_index];
}

//-------------------------------------------------------------------------------------------------
void MotorStringManager_RotateCounterClockwise(void)
{
  s_index++;
  PTAD_PTAD2 = portA2seq[s_index % BITS];
  PTAD_PTAD3 = portA3seq[s_index % BITS];
  PTBD_PTBD6 = portB6seq[s_index % BITS];
  PTBD_PTBD7 = portB7seq[s_index % BITS];
}

//-------------------------------------------------------------------------------------------------
void MotorStringManager_RotateClockwise(void)
{
  s_index--;
  PTAD_PTAD2 = portA2seq[s_index % BITS];
  PTAD_PTAD3 = portA3seq[s_index % BITS];
  PTBD_PTBD6 = portB6seq[s_index % BITS];
  PTBD_PTBD7 = portB7seq[s_index % BITS];
}
