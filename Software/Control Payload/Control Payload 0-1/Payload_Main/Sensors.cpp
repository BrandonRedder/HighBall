#include <SparkFun_MS5803_I2C.h>
#include <Wire.h>

#include "Sensors.h"
#include <stdlib.h>
#include <string.h>
#include <TextFiner.h>

// Temperature Sensor {{{
#define TEMPERATURE_DEFAULT_CAL 100.0
#define TEMPERATURE_DEFAULT_PIN A2
// Default Constructor, assumes 100 cal and pin A2
temperature_sensor::temperature_sensor() {
  set_cal(TEMPERATURE_DEFAULT_CAL); // 10 mV <-> 1 Kelvin
  set_pin(TEMPERATURE_DEFAULT_PIN);
}

// Constructor, set cal and pin #
temperature_sensor::temperature_sensor(float _cal, int _pin) {
  set_cal(_cal);
  set_pin(_pin);
}

float temperature_sensor::read_temp() {
  /* Reads the analog output level and converts to temperature data
   *
   * Returns
   * -------
   *  float: Temperature measured by the sensor
   */
  float voltage = (5 / 1024) * ((float)analogRead(get_pin()));
  return(voltage * get_cal());
}

// get functions for each member variable
float temperature_sensor::get_cal() {return(calibration);}

int temperature_sensor::get_pin() {return(pin);}

// set functions for each member variable
void temperature_sensor::set_cal(float _cal) {
  calibration = _cal;
  return;
}

void temperature_sensor::set_pin(int _pin) {
  pin = _pin;
  return;
}

// }}}
// Pressure Sensor {{{
//#define PRESSURE_DEFAULT_ADDR 0x76
// Default constructor, assumes address=0x76
pressure_sensor::pressure_sensor() : sensor(ADDRESS_HIGH) {
  //initialize_sensor();
}

// Constructor, user provided address
pressure_sensor::pressure_sensor(ms5803_addr _addr) : sensor(_addr) {
  //initialize_sensor();
}

// Constructor, user provided sensor
/*pressure_sensor::pressure_sensor(MS5803& _sensor) {
  // This is not the recommended method of interacting with this class.
  // Provided for edge use cases
  sensor = _sensor;
  initialize_sensor();
}*/

float pressure_sensor::read_pressure() {
  /* Reads the current pressure
   *
   * Returns
   * -------
   *  float: current pressure read by the sensor
   */
  return(sensor.getPressure(ADC_4096));
}

float pressure_sensor::find_altitude() {
  /* Determines the current altitude from a baseline
   *
   * Returns
   * -------
   *  float: current altitude above baseline in feet
   */
  float P = read_pressure();
  // altitude calculation from sparkfun example
  float alt_meters = 44330.0*(1-pow(P/get_baseline(),1/5.255));
  float alt_feet = alt_meters * 3.28084;
  return(alt_feet);
}
/*
void pressure_sensor::create_sensor(ms5803_addr _addr) {
  /* Create the sensor and initialize it
   *
   * Inputs
   * ------
   *  _addr: int representing the address of the sensor

  // TODO: test whether this correctly sets the sensor variable
  MS5803 sensor(_addr);
  initialize_sensor();
}*/

void pressure_sensor::initialize_sensor() {
  /* Initialize the sensor and print an error if it failed to initialize
   */
  sensor.reset();
  int counter = 0;
  while (sensor.begin() && counter < 5) {
    Serial.println("Pressure Sensor failed to initialize");
    sensor.reset();
    delay(500);
    counter++;
    if (counter == 5) {
      Serial.println("Pressure Sensor failed 5x, aborting!");
      // while(1);
    }
  } // end while
  Serial.println("Pressure Sensor initialized");
  set_baseline(sensor.getPressure(ADC_4096));
}

// get functions for each member variable
ms5803_addr pressure_sensor::get_addr() {return(addr);}

//MS5803& pressure_sensor::get_sensor() {return(sensor);}

float pressure_sensor::get_baseline() {return(baseline);}

// set functions for each member variable
void pressure_sensor::set_addr(ms5803_addr _addr) {
  addr = _addr;
}

void pressure_sensor::set_baseline(float _baseline) {
  baseline = _baseline;
}

// }}}

// IMU {{{
#define IMU_DEFAULT_TX 16
#define IMU_DEFAULT_RX 17

IMU::IMU(){
  /* Constructor, tests serial communication and initializes the data to 0
   */
  set_Data(0, 0, 0);
}

TextFinder finder(Serial2);
const int numOfFields = 3;
float Array[numOfFields];

int IMU::comms_Test() {
  /* Check communication between the IMU arduino and the main arduino
   *
   * Returns
   * -------
   *  int - 1 corresponds to sucess, 0 corresponds to failure
   */
  //int time = millis();
  //Serial2.begin(9600);
  Serial2.print("#c");
  // check communication with IMU
  boolean found_CommsHeader = finder.find("C");

  if (found_CommsHeader) {
    float testVal = finder.getFloat();
    if (testVal == 2.0) {
      return(1);
    }
  }

  return(0);
  /*while((millis()-time) < 180000){
    if(Serial2.read() == '1'){//need to change back to '1'
      Serial2.print('0');
      return(1);
    }
    Serial2.print('1');
    delay(500);
  }
  return(0);
  */
}

IMU_Data IMU::read_IMU(){
  /* Read current imu data for acceleration and direction
   *
   * Returns
   * -------
   *  IMU_Data - data corresponding to the current values on the IMU
   */
  Serial2.write("#f"); //request Data
  int fieldIndex = 0;
  boolean found_HeaderChar = finder.find("H");

  if(found_HeaderChar){
    while(fieldIndex < numOfFields){
      Array[fieldIndex++] = finder.getFloat();
    }
    set_Data(Array[0], Array[1], Array[2]);
    Serial.println("IMU Received Data");
    Serial.println(String(Array[0]) + ', ' +  String(Array[1]) + ', ' + String(Array[2]);
  }

  //bool reading = false;
  //String receivedValues = "";
  //String delimiter = ",";
  //float numbers[3] = {0.0,0.0,0.0};
  //int numIdx=0;
  /*
  int time = millis();
  //Request Data
  Serial2.print('2');
  Serial.println("Inside read_IMU");
  while((millis()-time) < 180000) {
    if (Serial2.available() > 0) {
      Serial.println((char)Serial2.read());
      /*if(Serial2.read() == '<') {
        numbers[0] = Serial2.readStringUntil(',').toFloat();
        numbers[1] = Serial2.readStringUntil(',').toFloat();
        numbers[2] = Serial2.readStringUntil('>').toFloat();
        set_Data(numbers[0], numbers[1], numbers[2]);
        Serial2.print('0');
        return(get_Data());
      } else {
        delay(500);
      }
    } else {
      Serial2.print('2');
    }
  }*/

/*
  if(Serial2.available() > 0){
    int num = Serial2.read();
    //Serial.println(num);
    if (reading == true){
      if ((char) num != '>'){
        //if new num(character) is not at the end, append string
        Serial.println("This is num" + String(num));
        receivedValues += (char)num;
      }//end if end
      else{
        reading = false;
        receivedValues += '\0';
      }//done reading
    }
    //check for start character
    else if ((char) num == '<'){
      reading = true;
    }
  }
  Serial.println("String of receivedValues" + receivedValues);
  int pos = 0;
  String token;
  while((pos = receivedValues.indexOf(delimiter)) != -1){
    token = receivedValues.substring(0,pos);
    numbers[numIdx] = token.toFloat();
    numIdx++;
    receivedValues.remove(0,pos);
  }
  set_Data(numbers[0], numbers[1], numbers[2]);
*/
}

void IMU::set_Data(float accelU, float accelH, float dir){
  data.accelUp = accelU;
  data.accelHoriz = accelH;
  data.direction = dir;
}

IMU_Data IMU::get_Data(){
  return(data);
}

void IMU::initialize_IMU() {
  Serial.println("Initializing IMU");
  Serial2.begin(9600);
  int status = comms_Test();
  if(status == 1){
    Serial.println("IMU Communication Success");
  }
  else{
    Serial.println("FAILED! IMU Communications.");
  }
  Serial2.flush();
}

// }}}

// GPS {{{
#define GPS_DEFAULT_ADDR 0x10
#define GPS_DEFAULT_WAKE 22
#define GPS_DEFAULT_RESETN 23
#define GPS_DEFAULT_INT 2
// Default Constructor, assumes default addr is 0x10
GPS::GPS() {
  set_addr(GPS_DEFAULT_ADDR);
  set_wake(GPS_DEFAULT_WAKE);
  set_reset(GPS_DEFAULT_RESETN);
  set_int(GPS_DEFAULT_INT);
}

// Constructor, provide address
GPS::GPS(int _addr, int _wake, int _reset, int _int) {
  set_addr(_addr);
  set_wake(_wake);
  set_reset(_reset);
  set_int(_int);
}

GPS_Data GPS::read_GPS() {
  /* Update current position and then return that position
   *
   * Returns
   * -------
   *  GPS_Data: struct containing altitude, longitude, and latitude
   */
  digitalWrite(get_wake(), HIGH); // Wake GPS
  delay(100);

  //while (!digitalRead(get_int())); // Wait for GPS to be ready
  while (myGPS.available()) { // available() returns # of new bytes that can be read
    GPS_Parser.encode(myGPS.read()); // Give data to parser
  }
  /*if (GPS_Parser.location.isValid()){
    Serial.println(String(GPS_Parser.altitude.feet()) + String(GPS_Parser.location.lat()) + String(GPS_Parser.location.lng()) + String(GPS_Parser.satellites.value()));
  }
  else{
    Serial.println("Location not valid");
    Serial.println(String(GPS_Parser.date.day()));
  }*/
  set_alt(GPS_Parser.altitude.feet());
  set_lat(GPS_Parser.location.lat());
  set_long(GPS_Parser.location.lng());
  set_sats(GPS_Parser.satellites.value());

  digitalWrite(get_wake(), LOW); // return to low power mode

  return(get_data());
}

void GPS::create_GPS() {
  /* Create an initialize the GPS objects
   */
  I2CGPS myGPS;
  TinyGPSPlus GPS_Parser;
  initialize_GPS();
}

void GPS::initialize_GPS() {
  pinMode(get_reset(), OUTPUT);
  pinMode(get_wake(), OUTPUT);
  pinMode(get_int(), INPUT);

  digitalWrite(get_reset(), HIGH);
  digitalWrite(get_wake(), HIGH);

  int counter = 0;
  Serial.println("Initializing GPS");
  while (myGPS.begin() && counter < 5) {
    digitalWrite(get_reset(), HIGH);
    Serial.println("GPS failed to initialize");
    delay(500);
    counter++;
    if (counter == 5) {
      Serial.println("GPS failed 5x, FAILING!");
      // while(1);
    } // end if
  } // end while
  Serial.println("GPS Initialized");
}

// get functions
int GPS::get_addr() {return(addr);}

int GPS::get_wake() {return(wake);}

int GPS::get_reset() {return(reset);}

int GPS::get_int() {return(interact);}

GPS_Data GPS::get_data() {return(data);}

double GPS::get_alt() {return(data.altitude);}

double GPS::get_lat() {return(data.latitude);}

double GPS::get_long() {return(data.longitude);}

uint32_t GPS::get_sats() {return(data.satellites);}

// set functions
void GPS::set_addr(int _addr) {
  addr = _addr;
}

void GPS::set_wake(int _wake) {
  wake = _wake;
}

void GPS::set_reset(int _reset) {
  reset = _reset;
}

void GPS::set_int(int _int) {
  interact = _int;
}

void GPS::set_data(GPS_Data _data) {
  data = _data;
}

void GPS::set_alt(double _alt) {
  data.altitude = _alt;
}

void GPS::set_lat(double _lat) {
  data.latitude = _lat;
}

void GPS::set_long(double _long) {
  data.longitude = _long;
}

void GPS::set_sats(uint32_t _sats) {
  data.satellites = _sats;
}

// }}}
// Battery Voltage {{{
#define Battery A3
#define Battery_Divider 1.15385
#define Battery_Max 12.6
#define Battery_Min 8.25

float check_battery(void)
{
  /* Measurement of the battery health */
  float Battery_Measurement = 5 * ((float)analogRead(Battery))/1024;
  // TODOC: This equation is not obvious at first glance, provide some
  // explanation about what is doing.
  return(100 * (Battery_Divider * Battery_Measurement - Battery_Min) /
         (Battery_Max - Battery_Min));
}
