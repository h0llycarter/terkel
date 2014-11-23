#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     elbow,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     conveyor,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     driveRearRight, tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     driveFrontRight, tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     driveRearLeft, tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     driveFrontLeft, tmotorTetrix, PIDControl, encoder)
#pragma config(Servo,  srvo_S1_C4_1,    roller,               tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

#define SERVO_ROLLER_UP    48
#define SERVO_ROLLER_DOWN  137

static int drive_multiplier = 1;

typedef enum conveyor_state_ {
    CONVEYOR_OFF,
    CONVEYOR_FORWARD,
    CONVEYOR_BACKWARD,
} conveyor_state_t;



typedef enum joystick_event_ {
    RIGHT_TRIGGER_UP = 6,
    RIGHT_TRIGGER_DOWN = 8,
    LEFT_TRIGGER_UP = 5,
    LEFT_TRIGGER_DOWN = 7,
    BUTTON_ONE = 1,
    BUTTON_FOUR = 4,
    BUTTON_TEN	= 10,
} joystick_event_t;

conveyor_state_t conveyor_state;

bool debounce;

task debounceTask()
{
    debounce = true;
    wait1Msec(500);
    debounce = false;
}

void all_stop()
{
    motor[driveFrontLeft] = 0;
    motor[driveRearLeft] = 0;
    motor[driveFrontRight] = 0;
    motor[driveRearRight] = 0;
}

void conveyor_enter_state(conveyor_state_t state)
{
    conveyor_state = state;

    switch (state) {
    case CONVEYOR_OFF:
        motor[conveyor] = 0;
        break;
    case CONVEYOR_FORWARD:
        motor[conveyor] = 100;
        break;
    case CONVEYOR_BACKWARD:
        motor[conveyor] = -100;
        break;
    }
}



void initializeRobot()
{
    conveyor_enter_state(CONVEYOR_OFF);
    servo[roller]=SERVO_ROLLER_DOWN;

    all_stop();

    return;
}

void handle_joy1_btn1()
{
    switch (conveyor_state) {
    case CONVEYOR_FORWARD:
        conveyor_enter_state(CONVEYOR_OFF);
        break;
    case CONVEYOR_BACKWARD:
        conveyor_enter_state(CONVEYOR_FORWARD);
        break;
    case CONVEYOR_OFF:
        conveyor_enter_state(CONVEYOR_FORWARD);
        break;
    }
}

void handle_joy1_btn4()
{
    switch (conveyor_state) {
    case CONVEYOR_FORWARD:
        conveyor_enter_state(CONVEYOR_BACKWARD);
        break;
    case CONVEYOR_BACKWARD:
        conveyor_enter_state(CONVEYOR_OFF);
        break;
    case CONVEYOR_OFF:
        conveyor_enter_state(CONVEYOR_BACKWARD);
        break;
    }
}

void handle_joy1_btn10()
{

			servo[roller] = SERVO_ROLLER_UP;


}


void handle_joy1_event(joystick_event_t event)
{
    switch (event) {
    case BUTTON_ONE:
        handle_joy1_btn1();
        break;
    case BUTTON_FOUR:
        handle_joy1_btn4();
        break;
    case BUTTON_TEN:
    		handle_joy1_btn10();
    		break;
    }

    startTask(debounceTask);
}



task main()
{
    short right_y;
    short left_y;

    debounce = false;

    initializeRobot();

    waitForStart();   // wait for start of tele-op phase

    // StartTask(endGameTimer);

    while (true) {

        getJoystickSettings(joystick);

        if (!debounce) {
	        if (joy1Btn(Btn1)) {
	            handle_joy1_event(BUTTON_ONE);
	        } else if (joy1Btn(Btn4)) {
	            handle_joy1_event(BUTTON_FOUR);
	        } else if (joy1Btn(Btn10)) {
	        		handle_joy1_event(BUTTON_TEN);
	        }

	        if (joy1Btn(Btn6)) {
	        	motor[elbow]=100;
	        } else if (joy1Btn(Btn8)) {
	        	motor[elbow]=-100;
	        } else {
	        	motor[elbow]=0;
	        }
        }

        //if (drive_multiplier) {
            //right_y = joystick.joy1_y2;
            //left_y = joystick.joy1_y1;
        //} else {
            left_y = joystick.joy1_y1;
            right_y = joystick.joy1_y2;
        //}

        if (abs(right_y) > 20) {
	    	motor[driveFrontRight] = drive_multiplier * right_y;
	    	motor[driveRearRight] = drive_multiplier * right_y;
		}
		else {
		    motor[driveFrontRight] = 0;
		    motor[driveRearRight] = 0;
		}

        if (abs(left_y) > 20) {
		    motor[driveFrontLeft] = drive_multiplier * left_y;
		    motor[driveRearLeft] = drive_multiplier * left_y;
		}
		else
		{
		    motor[driveFrontLeft] = 0;
		    motor[driveRearLeft] = 0;
		}
    }
}
