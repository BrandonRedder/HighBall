
#include "Actuators.h"

Servo HeliumServo;
Servo BallastServo;

unsigned long HeliumStartTime;
bool HeliumOpen = false;

void setup_Actuators(void) {
  HeliumServo.attach(7);
  HeliumServo.write(HeliumStartAngle);
  BallastServo.attach(6);
  BallastServo.write(BallastStartAngle);
}

// Open helium vent by given percentage (0-1) for a given time period
int openHeliumServo (float percentOpen) {
  double degree;
  if (!HeliumOpen) {
    HeliumOpen = true;
    HeliumStartTime = millis();
  }
  percentOpen = percentOpen*2;
  if (percentOpen <= 1) {
    degree = 180*acos(percentOpen)/PI;
  } else {
    degree = 180*acos(percentOpen-1)/PI+90;
  }
  HeliumServo.write(HeliumStartAngle+degree);
  return (millis() - HeliumStartTime);
}

void closeHeliumServo (void) {
  HeliumServo.write(HeliumStartAngle);
  HeliumOpen = false;
  HeliumStartTime = 0;
}

int oscillateHeliumServo (int period, int repetitions, int degree) {
  if (!HeliumOpen) {
    for(int i; i < repetitions; i++) {
      HeliumServo.write(HeliumStartAngle+degree);
      delay(period);
      HeliumServo.write(HeliumStartAngle);
    }
    return 1;
  } else {
    return -1;
  }
}

int readHeliumServo (void) {
  return HeliumServo.read();
}

void setBallastServo (int angle) {
  BallastServo.write(angle);
}

void oscillateBallastServo (int period, int repetitions, int degree) {
    for(int i; i < repetitions; i++) {
      BallastServo.write(BallastStartAngle+degree);
      delay(period);
      BallastServo.write(BallastStartAngle);
    }
}

int readBallastServo (void) {
  return BallastServo.read();
}

