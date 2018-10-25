#include "Sensors.h"
#include "Actuators.h"
#include "Communications.h"
#include "Control.h"
#include "State_Machine.h"
#include <Servo.h>

#define PRESSURE_01_ADR 0x76
#define PRESSURE_02_ADR 0x77
#define AUTO 1
#define MANUAL 2

#define HELIUM_SERVO 7
#define BALLAST_SERVO 6


//temperature_sensor temp;
pressure_sensor pressure1(ADDRESS_HIGH);
pressure_sensor pressure2(ADDRESS_LOW);
//IMU imu;
GPS gps;
//altitude_control control;
// Servo Setup
Servo helium_servo;
Servo ballast_servo;
int helium_pos = 0;
int ballast_pos = 0;

unsigned long controlTime;
unsigned long conditionTime;
helium_ballast action;
float altitude;
int mode = MANUAL;

float temperature;
float altitude1;
float altitude2;
float velocity;
IMU_Data imu_data;
GPS_Data gps_data;

void setup()
{
  Serial.begin(9600);

  Serial.println("Before Pressure");
  pressure1.initialize_sensor();
  pressure2.initialize_sensor();
  Serial.println("After Pressure");

  Serial.println("Before GPS");
  gps.create_GPS();
  Serial.println("After GPS");

  /* Serial.println("Before IMU"); */
  /* imu.initialize_IMU(); */
  /* Serial.println("After IMU") */

  helium_servo.attach(HELIUM_SERVO);
  ballast_servo.attach(BALLAST_SERVO);

  controlTime = millis();
  conditionTime = controlTime;
  altitude =  altitudeCalc(pressure1.find_altitude(), pressure2.find_altitude());
}

#define NO_ACTION 0
#define DROP_BALLAST 1
#define RELEASE_HELIUM 2


void loop()
{
  //Serial.println("in loop");
  
  //TODO: check communication (maybe output a struct that contains changes (manual mode, cutdown, etc))
  //TODO: deal with mode change
  //TODO: deal with cutdown
  //TODO: deal with anything else that would change
  //check current conditions

  if((millis() - conditionTime) >= 5*1000){//should we make this a variable time?
    //temperature = temp.read_temp();
    altitude1 = pressure1.find_altitude();
    altitude2 = pressure2.find_altitude();
    Serial.println("");
    Serial.println("Pressure Sensor Altitude 1 = " + String(altitude1));
    Serial.println("Presure Sensor Altitude 2 = " + String(altitude2));
    velocity = verticalVelocityCalc(altitudeCalc(altitude1, altitude2), altitude, millis(), conditionTime);
    Serial.println("Pressure Sensor Velocity = " + String(velocity));
    conditionTime = millis();
    altitude = altitudeCalc(altitude1, altitude2);//need to do error checking, this was a quick fix
    Serial.println("Pressure Sensor Altitude = " + String(altitude));

    //imu_data = imu.read_IMU();
    //Serial.println("IMU Upward Accel = " + String(imu_data.accelUp));
    //Serial.println("IMU Horizontal Accel = " + String(imu_data.accelHoriz));
    //Serial.println("IMU Direction = " + String(imu_data.direction));

    gps_data = gps.read_GPS();
    Serial.println("GPS Altitude = " + String(gps_data.altitude))
    Serial.println("GPS latitude = " + String(gps_data.latitude))
    Serial.println("GPS longitude = " + String(gps_data.longitude))
    Serial.println("GPS satellites = " + String(gps_data.satellites))
    Serial.println("");
  }

  //run control algorithm every 5 minutes
  if(((millis()-controlTime) >= 5*60*1000) && mode == AUTO){//should we make this a variable time?
    //action = control.get_action(altitude, velocity, imu_data.accelUp);
    //float battery_level = check_battery();
  } else if (mode == MANUAL) {
    //action = getCommunicationAction(); // TODO: Kyle can you interface the communciation override here?
  }

  //act on action
  //action TODO: release ballast or helium
}

float verticalVelocityCalc(float altitudeNew, float altitudeOld, unsigned long timeNew, unsigned long timeOld){
  float velocity = (altitudeNew - altitudeOld)/(timeNew-timeOld);
  return(velocity);
}

float altitudeCalc(float alt1, float alt2) {
  float alt = (alt1 + alt2)/2;
  return(alt);
}

void open_close_helium() {
  if (helium_pos == 0) {
    helium_servo.write(180);
    helium_pos = 180;
  } else if (helium_pos == 180) {
    helium_servo.write(0);
    helium_pos = 0;
  }
}

void release_ballast(int n) {
  // n = number of drops
  for (int i = 0; i < n; i++) {
    if(ballast_pos == 0) {
      ballast_servo.write(90);
      ballast_pos = 90;
      delay(1350);
    } else if(ballast_pos == 90) {
      ballast_servo.write(0);
      ballast_pos = 0;
      delay(1350);
    }
  }
}
