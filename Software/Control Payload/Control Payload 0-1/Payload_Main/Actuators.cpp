
#include "Actuators.h"

Servo HeliumServo;
#define BallastPin 6

void setup_Actuators(void) {
  HeliumServo.attach(7);
  HeliumServo.write(HeliumStartAngle);
  pinMode(BallastPin, OUTPUT);
  digitalWrite(BallastPin, 0);
}

// Open helium vent by given percentage (0-1) for a given time period
void openHeliumServo (struct Helium_Data* data, float percentOpen) {
  data->position = percentOpen;
  double degree;
  if (!data->open) {
    data->open = true;
  }
  percentOpen = percentOpen * 2;
  if (percentOpen <= 1) {
    degree = 180 * asin(percentOpen) / PI;
  } else {
    degree = 180 * asin(percentOpen - 1) / PI + 90;
  }
  HeliumServo.write(HeliumStartAngle + degree);
}

void closeHeliumServo (struct Helium_Data* data) {
  data->position = 0;
  HeliumServo.write(HeliumStartAngle);
  data->open = false;
  data->ventTime = millis();
}

void oscillateHeliumServo (struct Helium_Data* data, int period, int repetitions, int degree) {
  if (!data->open) {
    for (int i; i < repetitions; i++) {
      HeliumServo.write(HeliumStartAngle + degree);
      delay(period);
      HeliumServo.write(HeliumStartAngle);
    }
  } else {
    float start_position = data->position;
    HeliumServo.write(HeliumStartAngle);
    for (int i; i < repetitions; i++) {
      HeliumServo.write(HeliumStartAngle + degree);
      delay(period);
      HeliumServo.write(HeliumStartAngle);
    }
    openHeliumServo ( data, start_position);
  }
}

void runBallastServo (void) {
  analogWrite(BallastPin, 243);
  delay(200);

  analogWrite(BallastPin, 193);
  delay(400);

  analogWrite(BallastPin, 154);
  delay(100);

  digitalWrite(BallastPin, 0);
  delay(500);
}

