#ifndef ACTUATORS_H
#define ACTUATORS_H 

#include <Arduino.h>
#include <Servo.h>

//Constants
#define HeliumStartAngle 0
#define BallastStartAngle 0 
#define MaxHeliumVentTime 10000

//Functions
//Main Setup Function
void setup_Actuators(void);
int openHeliumServo (float percentOpen);
void closeHeliumServo (void);
int oscillateHeliumServo (int period, int repetitions, int degree);
int readHeliumServo (void);
void setBallastServo (int angle);
void oscillateBallastServo (int period, int repetitions, int degree);
int readBallastServo (void);
#endif /* ifndef ACTUATORS_H */
