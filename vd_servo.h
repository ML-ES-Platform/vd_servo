/*
 * vd_servo.h
 *
 *  Created on: May 13, 2020
 *      Author: Andrei Bragarenco
 */

#ifndef _VD_SERVO_H_
#define _VD_SERVO_H_

#include "vd_servo_cfg.h"

#ifndef VD_SERVO_CONFIG
enum VD_SERVO_IdType {VD_SERVO_CHANNEL_NR_OF = 0 };
#endif


#define SERVO_IDL 1
#define SERVO_ACC 2
#define SERVO_BRK 3
#define SERVO_NAV 4
#define SERVO_FWD ( 1)
#define SERVO_STP ( 0)
#define SERVO_BWD (-1)

#define SECOND_MS 1000.0

typedef struct VDSERVO_ChannelType_t {

	Std_ChannelIdType deviceChannelId = 0;
	Std_PhySetterType DeviceSetAngle = NULL;

	int8_t TASK_REQ_SERVO;
	int8_t servoNavState = SERVO_IDL;
	int8_t servoDirection = SERVO_STP;


	// GRD
	Std_PhyDataType ANGLE_MIN = 0;
	Std_PhyDataType ANGLE_MAX = 180;
	Std_PhyDataType startAngle = 0;
	Std_PhyDataType targetAngle = 0;
	Std_PhyDataType curentAngle = 0;
	Std_PhyDataType deltaAngle = 0;

	// GRD/SEC
	Std_PhyDataType SPEED_MAX = (90.0);
	Std_PhyDataType SPEED_MIN = (5.0);
	Std_PhyDataType currentSpeed = 0;
	Std_PhyDataType accCurrentSpeed = 0;
	Std_PhyDataType decCurrentSpeed = 0;


	Std_PhyDataType acceleratePhy = (60.0);
	Std_PhyDataType accelerateRec = (0);

//float servoAccelerate = (    5.0 / SERVO_TIME_COR );




} VDSERVO_ChannelType;


VDSERVO_ChannelType* VDSERVO_GetChannelRef(Std_ChannelIdType channelId);
Std_ReturnType VD_SERVO_ChannelSetup(Std_ChannelIdType servoChannelId, Std_ChannelIdType deviceChannelId);
Std_ReturnType VD_SERVO_SetPushMethod( Std_ChannelIdType channelId,  Std_PhySetterType SetAngle);
Std_ReturnType VDSERVO_GroupSetup(Std_ChannelIdType *srcIds, Std_ChannelIdType *targhetIds, uint8_t nr_of_channels);
Std_ReturnType VDSERVO_SetGroupPushMethod(Std_ChannelIdType *srcIds, Std_PhySetterType deviceSetter, uint8_t nr_of_channels);



Std_ReturnType VDSERVO_SpeedReset(Std_ChannelIdType channelId);
Std_ReturnType VDSERVO_SetRecurency(Std_ChannelIdType channelId, int rec);
Std_ReturnType VDSERVO_GroupSetRecurency(Std_ChannelIdType *srcIds, int recurency, uint8_t nr_of_channels);

Std_ReturnType VDSERVO_AngleSet(Std_ChannelIdType channelId, Std_PhyDataType angle);
Std_ReturnType VDSERVO_AngleSetByRef(VDSERVO_ChannelType *channelRef, Std_PhyDataType angle);
Std_ReturnType VDSERVO_AngleAdd(Std_ChannelIdType channelId, Std_PhyDataType angle);
Std_ReturnType VDSERVO_AngleAdd(VDSERVO_ChannelType *channelRef, Std_PhyDataType angle);
Std_PhyDataType VDSERVO_AngleGet(Std_ChannelIdType channelId);
Std_PhyDataType VDSERVO_AngleOutGet(Std_ChannelIdType channelId);


Std_ReturnType VDSERVO_AngleProcess(Std_ChannelIdType channelId);
Std_ReturnType VDSERVO_AngleProcess(VDSERVO_ChannelType *cnlRef);

Std_ReturnType VDSERVO_ChannelRunnable(void);



#endif /* _VD_SERVO_H_ */
