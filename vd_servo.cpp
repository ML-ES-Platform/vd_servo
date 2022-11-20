/*
 * vd_servo.cpp
 *
 *  Created on: May 13, 2020
 *      Author: Andrei Bragarenco
 */

#include "vd_servo.h"

VDSERVO_ChannelType VDSERVO_Channels[VD_SERVO_CHANNEL_NR_OF];


VDSERVO_ChannelType* VDSERVO_GetChannelRef(Std_ChannelIdType channelId) {
	VDSERVO_ChannelType *channelRef;
	if (channelId < VD_SERVO_CHANNEL_NR_OF) {
		channelRef = &VDSERVO_Channels[channelId];
	} else {
		channelRef = NULL;
	}
	return channelRef;

}

Std_ReturnType VD_SERVO_ChannelSetup(Std_ChannelIdType servoChannelId, Std_ChannelIdType deviceChannelId){
	Std_ReturnType error;
	if (servoChannelId < VD_SERVO_CHANNEL_NR_OF) {
		VDSERVO_ChannelType *channelRef = VDSERVO_GetChannelRef(servoChannelId);
		channelRef->deviceChannelId = deviceChannelId;
		error = E_OK;
	} else {
		error = E_NOT_OK;
	}
	return error;
}

Std_ReturnType VD_SERVO_SetPushMethod( Std_ChannelIdType channelId,  Std_PhySetterType SetAngle){
	Std_ReturnType error;
	if (channelId < VD_SERVO_CHANNEL_NR_OF) {
		VDSERVO_ChannelType *channelRef = VDSERVO_GetChannelRef(channelId);
		channelRef->DeviceSetAngle = SetAngle;
		error = E_OK;
	} else {
		error = E_NOT_OK;
	}
	return error;
}


Std_ReturnType VDSERVO_GroupSetup(Std_ChannelIdType *srcIds, Std_ChannelIdType *targhetIds, uint8_t nr_of_channels)
{
	Std_ReturnType error = E_OK;

	for (size_t i = 0; i < nr_of_channels; i++)
	{
		Std_ChannelIdType srcId = srcIds[i];
		Std_ChannelIdType targhetId = targhetIds[i];
		error += VD_SERVO_ChannelSetup(srcId, targhetId);
	}
	return error;
}

Std_ReturnType VDSERVO_SetGroupPushMethod(Std_ChannelIdType *srcIds, Std_PhySetterType deviceSetter, uint8_t nr_of_channels)
{
	Std_ReturnType error = E_OK;

	for (size_t i = 0; i < nr_of_channels; i++)
	{
		Std_ChannelIdType srcId = srcIds[i];
		error += VD_SERVO_SetPushMethod(srcId, deviceSetter);
	}
	return error;
}

Std_ReturnType VDSERVO_SetRecurency(Std_ChannelIdType channelId, int recurency){
	VDSERVO_ChannelType *cnlRef = VDSERVO_GetChannelRef(channelId);

	if(cnlRef == NULL)
		return E_NOT_OK;

	cnlRef->TASK_REQ_SERVO = recurency;
	cnlRef->accelerateRec = (cnlRef->acceleratePhy / (SECOND_MS / recurency));
	return E_OK;
}

Std_ReturnType VDSERVO_GroupSetRecurency(Std_ChannelIdType *srcIds, int recurency, uint8_t nr_of_channels)
{
	Std_ReturnType error = E_OK;

	for (size_t i = 0; i < nr_of_channels; i++)
	{
		Std_ChannelIdType srcId = srcIds[i];
		error += VDSERVO_SetRecurency(srcId, recurency);
	}
	return error;
}

Std_ReturnType VDSERVO_SpeedReset(Std_ChannelIdType channelId) {
	VDSERVO_ChannelType *cnlRef = VDSERVO_GetChannelRef(channelId);
	if(cnlRef == NULL)
		return E_NOT_OK;

	cnlRef->currentSpeed = 0;

	return E_OK;
}


Std_ReturnType VDSERVO_AngleSet(Std_ChannelIdType channelId, Std_PhyDataType angle) {

	VDSERVO_ChannelType *cnlRef = VDSERVO_GetChannelRef(channelId);

	return VDSERVO_AngleSetByRef(cnlRef, angle);

}

Std_ReturnType VDSERVO_AngleSetByRef(VDSERVO_ChannelType *cnlRef, Std_PhyDataType angle) {


	if(cnlRef == NULL)
		return E_NOT_OK;

	cnlRef->targetAngle = angle;

	if (cnlRef->targetAngle > cnlRef->ANGLE_MAX)
		cnlRef->targetAngle = cnlRef->ANGLE_MAX; //saturatie

	if (cnlRef->targetAngle < cnlRef->ANGLE_MIN)
		cnlRef->targetAngle = cnlRef->ANGLE_MIN; //saturatie

	cnlRef->accCurrentSpeed = cnlRef->currentSpeed;

	return E_OK;
}


Std_PhyDataType VDSERVO_AngleGet(Std_ChannelIdType channelId) {

	VDSERVO_ChannelType *cnlRef = VDSERVO_GetChannelRef(channelId);

	return cnlRef->targetAngle;

}

Std_PhyDataType VDSERVO_AngleOutGet(Std_ChannelIdType channelId) {

	VDSERVO_ChannelType *cnlRef = VDSERVO_GetChannelRef(channelId);

	return cnlRef->curentAngle;

}


Std_ReturnType VDSERVO_AngleAdd(Std_ChannelIdType channelId, Std_PhyDataType angle) {
	VDSERVO_ChannelType *cnlRef = VDSERVO_GetChannelRef(channelId);

	return VDSERVO_AngleAdd(cnlRef, angle);
}

Std_ReturnType VDSERVO_AngleAdd(VDSERVO_ChannelType *cnlRef, Std_PhyDataType angle) {

	if (cnlRef == NULL)
		return E_NOT_OK;
	else
		return VDSERVO_AngleSetByRef(cnlRef, cnlRef->targetAngle + angle);

}


Std_ReturnType VDSERVO_AngleProcess(Std_ChannelIdType channelId) {

	VDSERVO_ChannelType *cnlRef = VDSERVO_GetChannelRef(channelId);

	return VDSERVO_AngleProcess(cnlRef);
}


Std_ReturnType VDSERVO_AngleProcess(VDSERVO_ChannelType *cnlRef) {

  // evaluate remaining distance
	cnlRef->deltaAngle = cnlRef->targetAngle - cnlRef->curentAngle;

	if(cnlRef == NULL)
		return E_NOT_OK;

  //evaluate  direction
  if (cnlRef->deltaAngle > 0.02)cnlRef->servoDirection = SERVO_FWD;
  else if (cnlRef->deltaAngle < -0.02)cnlRef->servoDirection = SERVO_BWD;
  else cnlRef->servoDirection = SERVO_STP;

  // extract absolute value of the delta angle
  cnlRef->deltaAngle = abs(cnlRef->deltaAngle);

	if (cnlRef->servoDirection == SERVO_STP)
	{ // reset if stop is reached
		cnlRef->currentSpeed = 0;
		cnlRef->accCurrentSpeed = 0;
		cnlRef->decCurrentSpeed = 0;
		cnlRef->servoNavState = SERVO_IDL;
	}
	else
	{
		// detect Accelerate decelerate indicators
		cnlRef->accCurrentSpeed += cnlRef->accelerateRec;
		cnlRef->decCurrentSpeed = sqrt((double) (2 * cnlRef->deltaAngle * cnlRef->acceleratePhy));

		// detect servo Navigate State
		if ( min(cnlRef->accCurrentSpeed, cnlRef->decCurrentSpeed)>= cnlRef->SPEED_MAX)
		{
			cnlRef->servoNavState = SERVO_NAV;
			cnlRef->currentSpeed = cnlRef->SPEED_MAX;
		}
		else if (cnlRef->accCurrentSpeed <= cnlRef->decCurrentSpeed)
		{
			cnlRef->servoNavState = SERVO_ACC;
			cnlRef->currentSpeed += cnlRef->accelerateRec;
		}
		else if (cnlRef->accCurrentSpeed > cnlRef->decCurrentSpeed)
		{
			cnlRef->servoNavState = SERVO_BRK;
			cnlRef->currentSpeed -= cnlRef->accelerateRec;
		}

		cnlRef->currentSpeed = max(cnlRef->currentSpeed, cnlRef->SPEED_MIN);

	}


	// angle evolution
	cnlRef->curentAngle += cnlRef->servoDirection * (cnlRef->currentSpeed / (SECOND_MS / cnlRef->TASK_REQ_SERVO)) ;

	// on forwarding
	if (cnlRef->servoDirection == SERVO_FWD) {
		// saturate angle
		cnlRef->curentAngle = min(cnlRef->curentAngle, cnlRef->targetAngle);
	}
	// on backwarding
	else if (cnlRef->servoDirection == SERVO_BWD) {
		cnlRef->curentAngle = max(cnlRef->curentAngle, cnlRef->targetAngle);
		// on Stopping
	} else {
	}

  return E_OK;
}

#include "Arduino.h"

extern volatile int interruptCounter;
extern int CheckPoint_HERE;
Std_ReturnType VDSERVO_ChannelRunnable(void) {

	interruptCounter++;

	for (int channelId = 0; channelId < VD_SERVO_CHANNEL_NR_OF; ++channelId) {
		VDSERVO_ChannelType *channelRef = VDSERVO_GetChannelRef(channelId);
		CheckPoint_HERE++;
		VDSERVO_AngleProcess(channelRef);
	}

	return 0;
}

