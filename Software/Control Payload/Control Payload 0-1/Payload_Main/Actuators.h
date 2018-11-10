#ifndef ACTUATORS_H
#define ACTUATORS_H 

#include <Arduino.h>
#include <Servo.h>

//Constants
#define HeliumStartAngle 0

//Helium Servo
struct Helium_Data{
  float position;
  unsigned long ventTime;
  bool open;
};

//Functions
//Main Setup Function
void setup_Actuators(void);
void openHeliumServo (struct Helium_Data* data, float percentOpen);
void closeHeliumServo (struct Helium_Data* data);
void oscillateHeliumServo (struct Helium_Data* data, int period, int repetitions, int degree);
void runBallastServo (void);
#endif /* ifndef ACTUATORS_H */
