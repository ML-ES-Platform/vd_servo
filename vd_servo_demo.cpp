/*
 * dd_servo_demo.cpp
 *
 *  Created on: May 12, 2020
 *      Author: Andrei Bragarenco
 */

#include "ESW/dd_pca9685/dd_pca9685.h"
#include "ESW/dd_servo/dd_servo.h"
#include "ESW/vd_act_servo/vd_servo.h"
#include <ESW/vd_act_servo/vd_servo_demo.h>
#include "BSW/os_time_trig/os_time_trig.h"

#define PCA9685_PWM_FREQ 50 // Analog servos run at ~50 Hz updates


#define SECOND_MS 1000.0

#define TASK_REQ_SERVO 10
#define SERVO_TASK_OFFSET 100

#define TASK_PLOT_REQ 100
#define TASK_PLOT_OFFSET 100

#define TASK1_REQ 10000
#define TASK1_OFFSET 0


OSTT_TaskDescriptor_t VD_ServoRun_TaskDescriptor  = OSTT_TaskDescriptorDefault();
OSTT_TaskDescriptor_t VD_ServoPlot_TaskDescriptor = OSTT_TaskDescriptorDefault();
OSTT_TaskDescriptor_t VD_ServoTest_TaskDescriptor = OSTT_TaskDescriptorDefault();


Std_ReturnType ServoTestTask(void) {

	static int onOff = 0;

	if (onOff > 0) {
		VDSERVO_AngleSet(VD_SERVO_1, 0); //saturatie
		VDSERVO_SpeedReset(VD_SERVO_1);
		onOff = 0;
	} else {
		VDSERVO_AngleSet(VD_SERVO_1, 180); //saturatie
		VDSERVO_SpeedReset(VD_SERVO_1);
		onOff = 1;
	}

	return onOff;
}








void VDSERVO_ServoReportHeadPlot(void) {

  Serial.println("");
  Serial.print("NavState");
  Serial.print(" ");
  Serial.print("DeltaAngle");
  Serial.print(" ");
  Serial.print("TargetAngle");
  Serial.print(" ");
  Serial.print("CurentAngle");
  Serial.print(" ");
  Serial.print("Direction");
  Serial.print(" ");
  Serial.print("accCurrentSpeed");
  Serial.print(" ");
  Serial.print("decCurrentSpeed");
  Serial.print(" ");
  Serial.print("MaxSpeed");
  Serial.print(" ");
  Serial.print("MinSpeed");
  Serial.print(" ");
  Serial.print("currentSpeed");

  Serial.println("");

  return;

}

void VDSERVO_ServoReportHeadPlot_V2(void) {

  Serial.println("");
  Serial.print("NavState");
  Serial.print(" ");
  Serial.print("Direction");
  Serial.print(" ");
  Serial.print("DeltaAngle");
  Serial.print(" ");
  Serial.print("TargetAngle");
  Serial.print(" ");
  Serial.print("CurentAngle");
  Serial.print(" ");
  Serial.print("accCurrentSpeed");
  Serial.print(" ");
  Serial.print("decCurrentSpeed");
  Serial.print(" ");
  Serial.print("SPEED_MAX");
  Serial.print(" ");
  Serial.print("SPEED_MIN");
  Serial.print(" ");
  Serial.print("currentSpeed");

  Serial.println("");

  return;

}

#define PLOT

Std_ReturnType VDSERVO_ServoReportPlot(void) {

	VDSERVO_ChannelType *cnlRef = VDSERVO_GetChannelRef(VD_SERVO_1);

#ifdef  PLOT
  Serial.print(cnlRef->servoNavState * 10 + 50);
  Serial.print(" ");
  Serial.print(cnlRef->deltaAngle / 5);
  Serial.print(" ");
  Serial.print(cnlRef->targetAngle / 5 - 180 / 5);
  Serial.print(" ");
  Serial.print(cnlRef->curentAngle / 5 - 2 * 180 / 5);
  Serial.print(" ");
  Serial.print(cnlRef->servoDirection * 10 - 2 * 180 / 5 - 20 );
  Serial.print(" ");
  Serial.print(cnlRef->accCurrentSpeed  - 2 * 180 / 5 - 80);
  Serial.print(" ");
  Serial.print(cnlRef->decCurrentSpeed  - 2 * 180 / 5 - 80);
  Serial.print(" ");
  Serial.print(cnlRef->SPEED_MAX  - 2 * 180 / 5 - 80);
  Serial.print(" ");
  Serial.print(cnlRef->SPEED_MIN  - 2 * 180 / 5 - 80);
  Serial.print(" ");
  Serial.print(cnlRef->currentSpeed  - 2 * 180 / 5 - 100);
  Serial.print(" ");

  Serial.println("");

#elif
  Serial.print(cnlRef->servoNavState);
  Serial.print(" ");
  Serial.print(cnlRef->deltaAngle);
  Serial.print(" ");
  Serial.print(cnlRef->targetAngle);
  Serial.print(" ");
  Serial.print(cnlRef->curentAngle);
  Serial.print(" ");
  Serial.print(cnlRef->servoDirection  );
  Serial.print(" ");
  Serial.print(cnlRef->accCurrentSpeed);
  Serial.print(" ");
  Serial.print(cnlRef->decCurrentSpeed );
  Serial.print(" ");
  Serial.print(cnlRef->SPEED_MAX);
  Serial.print(" ");
  Serial.print(cnlRef->SPEED_MIN );
  Serial.print(" ");
  Serial.print(cnlRef->currentSpeed);
  Serial.print(" ");

  Serial.println("");

#endif

  return 0;
}

Std_ReturnType VDSERVO_ServoReportPlot_V2(void) {

	VDSERVO_ChannelType *cnlRef = VDSERVO_GetChannelRef(VD_SERVO_1);


  Serial.print(cnlRef->servoNavState * 10 + 50);
  Serial.print(" ");
  Serial.print(cnlRef->servoDirection * 10 + 50 );
  Serial.print(" ");
  Serial.print(cnlRef->deltaAngle / 5);
  Serial.print(" ");
  Serial.print(cnlRef->targetAngle / 5 - 180 / 5 - 5);
  Serial.print(" ");
  Serial.print(cnlRef->curentAngle / 5 - 180 / 5 - 5);
  Serial.print(" ");
  Serial.print(cnlRef->accCurrentSpeed /2 - 2 * 180 / 5 - 50);
  Serial.print(" ");
  Serial.print(cnlRef->decCurrentSpeed/2  - 2 * 180 / 5 - 50);
  Serial.print(" ");
  Serial.print(cnlRef->SPEED_MAX /2 - 2 * 180 / 5 - 50);
  Serial.print(" ");
  Serial.print(cnlRef->SPEED_MIN /2 - 2 * 180 / 5 - 50);
  Serial.print(" ");
  Serial.print(cnlRef->currentSpeed /2 - 2 * 180 / 5 - 50);
  Serial.print(" ");

  Serial.println("");

  return 0;
}



void vd_servo_demo_setup() {
	// put your setup code here, to run once:
	// Add your initialization code here
	Serial.begin(9600);
	Serial.println("SERVO demo !");
	Std_ReturnType error = E_OK;

	error = PCA9685_DeviceInit(PCA9685_PWM_FREQ);

	error += DD_PCA9685_ChannelSetup(PCA9685_PWM_1, 0);
	error += DD_PCA9685_ChannelSetup(PCA9685_PWM_2, 1);
	error += DD_PCA9685_ChannelSetup(PCA9685_PWM_3, 2);
	// error += DD_PCA9685_ChannelSetup(PCA9685_PWM_4, 3);

	Serial.print("DD pca9685 configured - Error : ");
	Serial.println(error);

	error += DD_SERVO_ChannelSetup(DD_SERVO_1, PCA9685_PWM_1);
	error += DD_SERVO_ChannelSetup(DD_SERVO_2, PCA9685_PWM_2);
	error += DD_SERVO_ChannelSetup(DD_SERVO_3, PCA9685_PWM_3);
	// error += DD_SERVO_ChannelSetup(DD_SERVO_4, PCA9685_PWM_4);

	error += DD_SERVO_SetPushMethod(DD_SERVO_1, PCA9685_WriteChannel);
	error += DD_SERVO_SetPushMethod(DD_SERVO_2, PCA9685_WriteChannel);
	error += DD_SERVO_SetPushMethod(DD_SERVO_3, PCA9685_WriteChannel);
	// error += DD_SERVO_SetPushMethod(DD_SERVO_4, PCA9685_WriteChannel);

	Serial.print("DD SERVO configured - Error : ");
	Serial.println(error);

	error += VD_SERVO_ChannelSetup(VD_SERVO_1, DD_SERVO_1);

	error += VD_SERVO_SetPushMethod(VD_SERVO_1, DDSERVO_SetAngle);

	Serial.print("VD SERVO configured - Error : ");
	Serial.println(error);

	//DDSERVO_SetAngle(DD_SERVO_1, 90);
	DDSERVO_SetAngle(DD_SERVO_2, 90);


	VD_ServoRun_TaskDescriptor.recurrence = TASK_REQ_SERVO;
	VD_ServoRun_TaskDescriptor.startupTimeout = SERVO_TASK_OFFSET;
	OSTT_TaskInit(VDSERVO_ChannelRunnable, &VD_ServoRun_TaskDescriptor);
	VDSERVO_SetRecurency(VD_SERVO_1, TASK_REQ_SERVO);


	VD_ServoPlot_TaskDescriptor.recurrence = TASK_PLOT_REQ;
	VD_ServoPlot_TaskDescriptor.startupTimeout = TASK_PLOT_OFFSET;
	OSTT_TaskInit(VDSERVO_ServoReportPlot_V2, &VD_ServoPlot_TaskDescriptor);
	VDSERVO_ServoReportHeadPlot_V2();


	VD_ServoTest_TaskDescriptor.recurrence = TASK1_REQ;
	VD_ServoTest_TaskDescriptor.startupTimeout = TASK1_OFFSET;
	OSTT_TaskInit(ServoTestTask, &VD_ServoTest_TaskDescriptor);

	OSTT_Init();
}



#define CMD_DELTA_ANGLE 1

void vd_servo_demo_loop() {
	// put your main code here, to run repeatedly:


	if (Serial.available()) {
		char cmd = Serial.read();

		// Command Interpreter
		switch (cmd) {
		case 'q':
			VDSERVO_AngleAdd(VD_SERVO_1, CMD_DELTA_ANGLE); // increment angle

			Serial.println("B1 1x UP");
			break;
		case 'w':
			VDSERVO_AngleAdd(VD_SERVO_1,-CMD_DELTA_ANGLE); //decrement angle

			Serial.println("B1 1x down");
			break;
		case 'a':
			VDSERVO_AngleAdd(VD_SERVO_1, CMD_DELTA_ANGLE*5); // increment angle

			Serial.println("B1 5x  up");
			break;
		case 's':
			VDSERVO_AngleAdd(VD_SERVO_1,-CMD_DELTA_ANGLE*5); //decrement angle

			Serial.println("B1 5x down");
			break;
		case 'z':
			VDSERVO_AngleAdd(VD_SERVO_1, CMD_DELTA_ANGLE*10); // increment angle

			Serial.println("B1 10x up");
			break;
		case 'x':
			VDSERVO_AngleAdd(VD_SERVO_1,-CMD_DELTA_ANGLE*10); //decrement angle

			Serial.println("B1 10x down");
			break;

		}
	}

	for (int channelId = 0; channelId < VD_SERVO_CHANNEL_NR_OF; ++channelId) {
		VDSERVO_ChannelType *channelRef = VDSERVO_GetChannelRef(channelId);

		if (channelRef) {
			 // Serial.print(" 1");
			channelRef->DeviceSetAngle(channelId, channelRef->curentAngle);
			//delay (5);
			 // Serial.print(" 2");
		}
	}

}
