#include "Sensors.h"
#include "Actuators.h"
#include "Communications.h"
#include "Control.h"
#include "State_Machine.h"

#define PRESSURE_01_ADR 0x76
#define PRESSURE_02_ADR 0x77
#define AUTO 1
#define MANUAL 2
SerialUSB.begin(115200);
while(!SerialUSB);
SerialUSB.println("USB Printing");
temperature_sensor temp;
pressure_sensor pressure1(PRESSURE_01_ADR);
pressure_sensor pressure2(PRESSURE_02_ADR);
IMU imu;
GPS gps;
altitude_control control;
unsigned long controlTime;
unsigned long conditionTime;
helium_ballast action;
float altitude;

void setup()
{
  controlTime = millis();
  conditionTime = controlTime();
  altitude =  altitudeCalc(pressure1.find_altitude(), pressure2.find_altitude());
}

#define NO_ACTION 0
#define DROP_BALLAST 1
#define RELEASE_HELIUM 2

void loop()
{
  //TODO: check communication (maybe output a struct that contains changes (manual mode, cutdown, etc))
  //TODO: deal with mode change
  //TODO: deal with cutdown
  //TODO: deal with anything else that would change
  //check current conditions
  if((millis() - conditionTime) >= 5*1000){//should we make this a variable time?
    float temp = temp.read_temp();
    float altitude1 = pressure1.find_altitude();
    float altitude2 = pressure2.find_altitude();
    float velocity = verticalVelocityCalc(altitudeCalc(altitude1, altitude2), altitude, millis(), conditionTime);
    conditionTime = millis();
    float altitude = altitudeCalc(altitude1, altitude2);//need to do error checking, this was a quick fix
    IMU_Data imu_data = imu.read_imu();
    GPS_Data gps_data = gps.read_GPS();
  }

  //run control algorithm every 5 minutes
  if(((millis()-controlTime) >= 5*60*1000) && mode == AUTO){//should we make this a variable time?
    action = control.get_action(altitude, velocity, imu_data.accelUp);
    float battery_level = check_battery();
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
