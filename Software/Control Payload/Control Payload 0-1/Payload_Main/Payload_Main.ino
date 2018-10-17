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
unsigned long sendTime;
unsigned long heliumTime;
int ballastCounter;
helium_ballast action;
float altitude;
struct Incoming_Data incoming;
struct Outgoing_Data outgoing;
bool manual_action;

void setup()
{
  controlTime = millis();
  conditionTime = controlTime;
  sendTime = controlTime;
  altitude =  altitudeCalc(pressure1.find_altitude(), pressure2.find_altitude());
  incoming.update_rate = (5*60);
  incoming.manual_amount = 0;
  manual_action = false;
  ballastCounter = 0;
}

#define NO_ACTION 0
#define DROP_BALLAST 1
#define RELEASE_HELIUM 2

void loop()
{
  //TODO: check communication (maybe output a struct that contains changes (manual mode, cutdown, etc)) -Preliminary Code Added
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
    conditionTime = millis();
  }
  
  // send message
  if((millis() - sendTime) >= incoming.update_rate*1000) {
    // Temperature Data
    outgoing.temperature = temp.read_temp();
    // Pressure Data
    outgoing.pressure;
    // Acceleration Data
    IMU_Data imu_data = imu.read_imu();
      convert_IMU (imu_data);
    // GPS Data
    float altitude1 = pressure1.find_altitude();
    float altitude2 = pressure2.find_altitude();
    outgoing.altitude = altitudeCalc(altitude1, altitude2);
    GPS_Data gps_data = gps.read_GPS();
    outgoing.latitude = gps_data.latitude;
    outgoing.longitude = gps_data.longitude;
    // Battery Data
    outgoing.BatteryPercentage = check_battery();
    // Velovity Data
    float velocity = verticalVelocityCalc(altitudeCalc(altitude1, altitude2), altitude, millis(), conditionTime);
    outgoing.vel_magnitude;
    outgoing.vel_theta;
    outgoing.vel_phi;
    // Ballast Data
    outgoing.ballast;
    // Helium Data
    outgoing.helium;
    // State Data
    outgoing.state;
    // Control Mode Data
    outgoing.control_mode;
    // Emergency Data
    outgoing.emergency;
    
    encode_message(&outgoing);
    call_iridium(10);
    delay(1000);
    decode_message(&incoming)
    sendTime = millis();
  }

  //run control algorithm every 5 minutes
  if(((millis()-controlTime) >= 5*60*1000) && mode == AUTO){  //should we make this a variable time?
    action = control.get_action(altitude, velocity, imu_data.accelUp);
    float battery_level = check_battery();
  } else if (incoming.manual_adjust) { // make manual adjustment
     manual_action = true;
     incoming.manual_adjust = false;
     if (incoming.manual_select) { // adjust helium
       heliumTime = millis();
       openHeliumServo (.75);
     } else { // adjust ballast
       ballastCounter = incoming.manual_amount;
     }
  }
   
  // keep helium open due to manual input
  if (((millis()-heliumTime) >= 1000*incoming.manual_amount) && manual_action) {
      closeHeliumServo();
      manual_action = false;
  }
  
  // drop ballast due to manual input
  if (manual_action && ballastCounter > 0) {
	  oscillateBallastServo (500, 1, 90);
	  ballastCounter--;
	  if (ballastCounter == 0) {
		  manual_action = false;
	  }
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

void convert_IMU (IMU_Data imu_data) {
  outgoing.acc_magnitude = sqrtf(powf(imu_data.accelUp,2) + powf(imu_data.accelHoriz,2));
  outgoing.acc_theta = imu_data.direction;
  outgoing.acc_phi = atan2(imu_data.accelUp,imu_data.accelHoriz);
}
