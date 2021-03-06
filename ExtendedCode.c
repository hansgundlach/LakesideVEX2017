#pragma config(Motor,  port5,           frontleftmotor, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           frontrightmotor, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port2,           rearleftmotor, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           rearrightmotor, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           handMotor,     tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)


bool doUseGyro = false;  // Enable gyroscopic sensor.
float gyroOffset = 90.0;  // Degrees CCW from +x axis robot faces at gyro = 0.



task userDriveHolo() {
  const TVexJoysticks kChY = Ch3;  // Y-axis joystick channel.
  const TVexJoysticks kChX = Ch4;  // X-axis joystick channel.
  const TVexJoysticks kChR = Ch1;  // Rotation joystick channel.
  const short kLoopDelay = 25;  // Milliseconds for loop delay.
  const ubyte kNumbOfWheels = 4;  // Number of drive wheels.
  const ubyte kNumbOfMotorsPerWheel = 2;  // Max number of motors per wheel.
  const tMotor kNone = -1;  // Used for indicating the lack of an additional motor.
  const tMotor kMotorPort[kNumbOfWheels][kNumbOfMotorsPerWheel] = {  // Drive motor ports/names.
    {port2, kNone},  // Front-left.
    {port3, kNone},  // Front-right.
    {port4, kNone},  // Back-left.
    {port5, kNone}  // Back-right.
  };

  word x, y, r;
  float gyro, radius, theta, a, b, wheelSpeed[kNumbOfWheels], topSpeed;

  while (true) {
  	//button alignmnet

  if(vexRT[Btn6U]==1)
  	//go up
		{
	motor[handMotor] = 126;
	wait1Msec(120);
	motor[handMotor] = 0;

	}
	if(vexRT[Btn6D] == 1){
		//go down

	 // button 8 and 9 will move up at the same time

	motor[handMotor] = - 126;
	wait1Msec(120);
	motor[handMotor] = 0;




}





    /* Collect joystick & sensor values. */
    x = vexRT[kChX];  // X component.
    y = vexRT[kChY];  // Y component.
    r = vexRT[kChR];  // Rotation.

    /* Convert joystick values to polar. */
    radius = sqrt(pow(x, 2) + pow(y, 2));  // r = sqrt(x^2 + y^2)
    theta = (atan2(y, x) * (180 / PI));  // t = arctan(y / x) converted from radians to degrees.

    theta -= gyro;  // Adjust for gyro angle.

    /* Calculate opposite-side speeds. */
    a = ((cosDegrees(theta) - sinDegrees(theta)) * radius);  // Front-left and back-right.
    b = ((cosDegrees(theta) + sinDegrees(theta)) * radius);  // Front-right and back-left.

    /* Set speeds, including rotation. */
    wheelSpeed[0] = (a + r);  // Front-left.
    wheelSpeed[1] = (b - r);  // Front-right.
    wheelSpeed[2] = (b + r);  // Back-left.
    wheelSpeed[3] = (a - r);  // Back-right.

    /* Normalize speeds. */
    topSpeed = 0;
    for (ubyte i = 0; i < kNumbOfWheels; i++) {
      if (abs(wheelSpeed[i]) > topSpeed) {
        topSpeed = abs(wheelSpeed[i]);  // Find highest desired speed.
      }
    }
    if(topSpeed > 127) {
      for(ubyte i = 0; i < kNumbOfWheels; i++) {
        wheelSpeed[i] /= (topSpeed / 127);  // Downscale all speeds so none are above 127.
      }
    }

    /* Update motor powers. */
    for (ubyte i = 0; i < kNumbOfWheels; i++) {  // Cycle through all wheels.
      for (ubyte j = 0; j < kNumbOfMotorsPerWheel; j++) {  // Cycle through all motors for each wheel.
        if (kMotorPort[i][j] != kNone) {  // Check existence of motor.
          motor[kMotorPort[i][j]] = (word) wheelSpeed[i];  // Update motor power.
        }
      }
    }
    sleep(kLoopDelay);
  }
}

task main() {
  startTask(userDriveHolo);

  while (true) {
    sleep(100);
  }
}
