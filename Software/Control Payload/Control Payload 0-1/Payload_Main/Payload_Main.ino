#include "Sensors.h"
#include "Actuators.h"
#include "Communications.h"
#include "Control.h"
#include "State_Machine.h"
#include <Servo.h>

#define PRESSURE_01_ADR 0x76
#define PRESSURE_02_ADR 0x77
#define AUTO 0
#define MANUAL 3

#define ControlPeriod 300

int ballastUsed = 0;
int heliumUsed = 0;

#define CUT_DOWN 5

#define d2r (M_PI / 180.0)

//temperature_sensor temp;

pressure_sensor pressure1(ADDRESS_HIGH);
pressure_sensor pressure2(ADDRESS_LOW);

//IMU imu;

GPS gps;

//altitude_control control;
altitude_control control;
helium_ballast action;

// servo setup
struct Helium_Data helium_servo;
int ballast_count;

unsigned long controlTime;
unsigned long conditionTime;
unsigned long sendTime;
//unsigned long timer1;
//bool hel = true;
//bool bal = true;

float temperature1=0;
float temperature2=0;
float temperature=0;

float prs1;
float prs2;
float prs;
float altitude;
float altitude1;
float altitude2;
float velocity;
//IMU_Data imu_data;
GPS_Data gps_data;

double initial_lat;     
double initial_long;

struct Incoming_Data incoming;
struct Outgoing_Data outgoing;
bool message_sent;

void setup()
{
  //Setup and turn off cutdown mechanism
  pinMode(CUT_DOWN,OUTPUT);
  analogWrite(CUT_DOWN,0);
  Serial.begin(9600);
  
  Serial.println("Before Pressure");
  pressure1.initialize_sensor();
  pressure2.initialize_sensor();
  Serial.println("After Pressure");

  Serial.println("Before GPS");
  gps.create_GPS();
  Serial.println("After GPS");
/*
  Serial.println("Before IMU");
  imu.initialize_IMU();
  Serial.println("After IMU");
*/
  
  setup_Actuators();
  helium_servo.ventTime = controlTime;
  helium_servo.position = 0;
  helium_servo.open = false;
  ballast_count = 0;

  prs1 = pressure1.read_pressure();
  prs2 = pressure2.read_pressure();
  altitude =  altitudeCalc(pressure1.find_altitude(prs1), pressure2.find_altitude(prs2));

  temperature1 = pressure1.read_temperature();
  temperature2 = pressure2.read_temperature();
  temperature = (temperature1 + temperature2) / 2.0;

  gps_data = gps.read_GPS();         
  initial_lat = gps.get_lat();    
  initial_long = gps.get_long();
  
  setup_Communications();
  incoming.update_rate = 120;
  incoming.manual_adjust = 0;
  incoming.control_mode = MANUAL;
  incoming.cutdown = 0;
  incoming.hel_alpha = 0;
  incoming.hel_beta = 0;
  incoming.hel_gamma = 0;
  incoming.bal_alpha = 0;
  incoming.bal_beta = 0;
  incoming.bal_gamma = 0;
  incoming.cutdown = 0;
  
  message_sent = false;

  controlTime = millis();
  conditionTime = controlTime;
  sendTime = controlTime;
//  timer1 = controlTime;
}


void loop()
{
  //Serial.println("in loop");
  
  //TODO: check communication (maybe output a struct that contains changes (manual mode, cutdown, etc))
  //TODO: deal with mode change
  //TODO: deal with cutdown
  //TODO: deal with anything else that would change
  //check current conditions

  if((millis() - conditionTime)/1000 >= 5){//should we make this a variable time?
    //temperature = temp.read_temp();

    gps_data = gps.read_GPS();
    Serial.println("GPS Altitude = " + String(gps_data.altitude));
    Serial.println("GPS latitude = " + String(gps_data.latitude));
    Serial.println("GPS longitude = " + String(gps_data.longitude));
    Serial.println("GPS satellites = " + String(gps_data.satellites));
    Serial.println("");

    prs1 = pressure1.read_pressure();
    prs2 = pressure2.read_pressure();
    altitude1 = pressure1.find_altitude(prs1);
    altitude2 = pressure2.find_altitude(prs2);
    Serial.println("");
    Serial.println("Pressure Sensor Altitude 1 = " + String(altitude1));
    Serial.println("Presure Sensor Altitude 2 = " + String(altitude2));
    Serial.println("Pressure Sensor Pressure 1 = " + String(prs1));
    //Serial.println("Pressure Sensor Baseline 1 = " + String(pressure1.get_baseline()));
    Serial.println("Pressure Sensor Pressure 2 = " + String(prs2));
    //Serial.println("Pressure Sensor Baseline 2 = " + String(pressure2.get_baseline()));
    velocity = verticalVelocityCalc(altitudeCalc(altitude1, altitude2), altitude, millis(), conditionTime);
    Serial.println("Pressure Sensor Velocity = " + String(velocity));
    conditionTime = millis();
    altitude = altitudeCalc(altitude1, altitude2);//need to do error checking, this was a quick fix
    Serial.println("Pressure Sensor Altitude = " + String(altitude));

    temperature1 = pressure1.read_temperature();
    temperature2 = pressure2.read_temperature();
    temperature = (temperature1 + temperature2) / 2.0;
    Serial.println("");
    Serial.println("Temperature 1 = " + String(temperature1));
    Serial.println("Temperature 2 = " + String(temperature2));
    Serial.println("Temperature = " + String(temperature));
/*
    imu_data = imu.read_IMU();
    Serial.println("IMU Upward Accel = " + String(imu_data.accelUp));
    Serial.println("IMU Horizontal Accel = " + String(imu_data.accelHoriz));
    Serial.println("IMU Direction = " + String(imu_data.direction));
*/
  }
  
  // send message
  if ((millis() - sendTime) / 1000 >= incoming.update_rate) {
    Serial.println("Start Send");
    // Temperature Data
    outgoing.temperature = temperature; // get temperature
    // Pressure Data

    if (abs(prs1 - prs2) > .25 * prs1) {
      if (abs(altitude1 - altitude) > abs(altitude2 - altitude)) {
        prs = prs2;
      } else {
        prs = prs2;
      }
    } else {
      prs = (prs1 + prs2) / 2;
    }

    outgoing.pressure = prs; // get pressure
    // Acceleration Data
    // GPS Data
    outgoing.altitude = gps_data.altitude;
    outgoing.latitude = gps_data.latitude;
    outgoing.longitude = gps_data.longitude;
    // Velovity Data
    outgoing.vel = velocity;
    // Ballast Data
    outgoing.ballast = ballastUsed;
    // Helium Data
    outgoing.helium = heliumUsed;
    // State Data
    outgoing.state = 0; // state machine defunct
    // Control Mode Data
    outgoing.control_mode = incoming.control_mode;
    // Emergency Data
    outgoing.emergency = 0; // no emergency settings

    encode_message(&outgoing);
    message_sent = call_iridium(10);
    delay(1000);

    bool new_data;
    new_data = decode_message(&incoming);

    if (new_data) {
      Serial.println("*");
      Serial.println("*");
      Serial.println("*");
      Serial.println("Altitude: " + String(incoming.altitude));
      Serial.println("Alt Buffer: " + String(incoming.altitude_buffer));
      Serial.println("Latitude: " + String(incoming.lat_deg));
      Serial.println("Longitude: " + String(incoming.long_deg));
      Serial.println("Update Rate: " + String(incoming.update_rate));
      Serial.println("Max Velocity: " + String(incoming.max_velocity));
      Serial.println("Temperature: " + String(incoming.temp));
      Serial.println("Control: " + String(incoming.control_mode));
      Serial.println("Manual Amount: " + String(incoming.manual_amount));
      
      Serial.println("Helium Alpha: " + String(incoming.hel_alpha));
      Serial.println("Helium Beta: " + String(incoming.hel_beta));
      Serial.println("Helium Gamma: " + String(incoming.hel_gamma));
      Serial.println("Ballast Alpha: " + String(incoming.bal_alpha));
      Serial.println("Ballast Beta: " + String(incoming.bal_beta));
      Serial.println("Ballast Gamma: " + String(incoming.bal_gamma));
      Serial.println("Cutdown: " + String(incoming.cutdown));
      Serial.println("*");
      Serial.println("*");
      Serial.println("*");
  
      control.set_min_altitude(incoming.altitude - incoming.altitude_buffer/2);
      control.set_max_altitude(incoming.altitude + incoming.altitude_buffer/2);
      
      control.set_min_velocity(incoming.max_velocity);
      // control.set_min_accel(incoming.altitude);
      
      control.set_bal_alpha(incoming.bal_alpha);
      control.set_bal_beta(incoming.bal_beta);
      control.set_bal_gamma(incoming.bal_gamma);
      control.set_hel_alpha(incoming.hel_alpha);
      control.set_hel_beta(incoming.hel_beta);
      control.set_hel_gamma(incoming.hel_gamma);
    }

    sendTime = millis();
  }

  //run control algorithm every 5 minutes
  if ((millis() - controlTime) / 1000 >= ControlPeriod && incoming.control_mode == AUTO) { //should we make this a variable time?
    // fill struct with ballast and helium scores
    //action = control.get_action(altitude, velocity, imu_data.accelUp);
    action = control.get_action(altitude, velocity, 0);
    if (action.helium > 0) {
      if (action.helium > ControlPeriod) {
        action.helium = ControlPeriod;
      }
      // set helium vent end time
      helium_servo.ventTime = (unsigned long)(action.helium*1000) + millis(); 
      // update helium used register
      heliumUsed = heliumUsed + action.helium;
      // open helium 80%
      openHeliumServo (&helium_servo, .8);
    } else if (action.ballast > 0) {
      if (action.ballast > ControlPeriod/2) {
        action.ballast= ControlPeriod/2;
      }
      // set count of how many ballast drops during this control cycle
      ballast_count = (int) (action.ballast);
    }
  } else if (incoming.control_mode == MANUAL && incoming.manual_adjust) {
    // unlatch manual adjust bit
    incoming.manual_adjust = false;
    // check whether adjustment is for helium or ballast
    if (incoming.manual_select) {
      // set count of how many ballast drops during this control cycle
      ballast_count = (int) (incoming.manual_amount);
      Serial.println("set ballast count");
    } else {
      // set helium vent end time
      helium_servo.ventTime = (unsigned long)(incoming.manual_amount*1000) + millis(); 
      // open helium 80%
      Serial.println("open helium");
      openHeliumServo (&helium_servo, 1);
      // update helium used register
      heliumUsed = heliumUsed + incoming.manual_amount;
    }
  }

//  if ((millis() - timer1)/1000 > 60  && hel )  {
//    hel = false;
//    Serial.println("adjust helium");
//    incoming.control_mode = MANUAL;
//    incoming.manual_adjust = true;
//    incoming.manual_select = false;
//    incoming.manual_amount = 5;
//  }
//
//  if ((millis() - timer1)/1000 > 70  && bal )  {
//    bal = false;
//    Serial.println("adjust ballast");
//    incoming.control_mode = MANUAL;
//    incoming.manual_adjust = true;
//    incoming.manual_select = true;
//    incoming.manual_amount = 5;
//  }

  // Drop ballast if currently requested
  if (ballast_count > 0) {
    // drop ballast and decrement the count
    runBallastServo();
    Serial.println(ballast_count);
    ballast_count = ballast_count - 1;
    // update helium used register
    ballastUsed = ballastUsed + 1;
  }

  // Close helium vent after alloted time
  if (helium_servo.ventTime < millis()  && helium_servo.open) {
    // close the helium vent after time runs out
    Serial.println("close helium");
    closeHeliumServo (&helium_servo);
  }
  
  // Cutdown after getting signal
  if (incoming.cutdown) {
    // cutdown
    cut_down();
  }
}

float verticalVelocityCalc(float altitudeNew, float altitudeOld, unsigned long timeNew, unsigned long timeOld){
  float velocity = (altitudeNew - altitudeOld) / (timeNew-timeOld) * 1000;
  return(velocity);
}

float altitudeCalc(float alt1, float alt2) {
  float alt = (alt1 + alt2)/2;
  return(alt);
}

void auto_cutdown(double initial_lat, double initial_long, double lat2, double long2)
{
    float dlong = (long2 - initial_long) * d2r;
    float dlat = (lat2 - initial_lat) * d2r;
    float a = pow(sin(dlat/2.0), 2) + cos(initial_lat*d2r) * cos(lat2*d2r) * pow(sin(dlong/2.0), 2);
    float c = 2 * atan2(sqrt(a), sqrt(1-a));
    float d = 3956 * c; 
  if(d>100.0)        //cutdown if distance is greater than 100.0 miles
  {
    cut_down();
  }  
}

void cut_down(){
  analogWrite(CUT_DOWN,105);
  delay(40000);
  analogWrite(CUT_DOWN,0);
}
