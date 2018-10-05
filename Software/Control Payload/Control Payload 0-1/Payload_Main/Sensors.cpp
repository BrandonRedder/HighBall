#include "Sensors.h"
#include <string>
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
#define PRESSURE_DEFAULT_ADDR 0x76
// Default constructor, assumes address=0x76
pressure_sensor::pressure_sensor() {
  set_addr(PRESSURE_DEFAULT_ADDR);
  create_sensor(get_addr());
}

// Constructor, user provided address
pressure_sensor::pressure_sensor(int _addr) {
  set_addr(_addr);
  create_sensor(get_addr());
}

// Constructor, user provided sensor
pressure_sensor::pressure_sensor(MS5803& _sensor) {
  // This is not the recommended method of interacting with this class.
  // Provided for edge use cases
  sensor = _sensor;
  initialize_sensor();
}

float pressure_sensor::read_pressure() {
  /* Reads the current pressure
   *
   * Returns
   * -------
   *  float: current pressure read by the sensor
   */
  return(get_sensor().getPressure(ADC_4096));
}

void pressure_sensor::create_sensor(int _addr) {
  /* Create the sensor and initialize it
   *
   * Inputs
   * ------
   *  _addr: int representing the address of the sensor
   */
  // TODO: test whether this correctly sets the sensor variable
  MS5803 sensor(_addr);
  initialize_sensor();
}

void pressure_sensor::initialize_sensor() {
  /* Initialize the sensor and print an error if it failed to initialize
   */
  get_sensor().reset();
  int counter = 0
  while (get_sensor().begin() && counter < 5) {
    serial.println("Pressure Sensor failed to initialize")
    get_sensor.reset();
    delay(500);
    counter++;
    if (counter == 5) {
      serial.println("Pressure Sensor failed 5x, aborting!")
    }
  } // end while
  set_baseline(get_sensor().getPressure(ADC_4096));
}

// get functions for each member variable
int pressure_sensor::get_addr() {return(addr);}

MS5803& pressure_sensor::get_sensor() {return(sensor);}

float pressure_sensor::get_baseline() {return(baseline);}

// set functions for each member variable
void pressure_sensor::set_addr(int _addr) {
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
  int status = comms_Test();
  if(status == '1'){
    SerialUSB.println("IMU Communication Success")
  }
  else{
    SerialUSB.println("FAILED! IMU Communications.");
  }
  IMU_Data _data;
  _data.accelUp = 0;
  _data.accelHoriz = 0;
  _data.direction = 0;
  set_data(_data);
}

int IMU::comms_Test() {
  int time = millis();
  Serial2.begin(9600);
  Serial2.print('1');
  // check communication with IMU
  while((millis()-time) < 180000){
    if(Serial2.read() == '1'){
      Serial2.print('0');
      return('1');
    }
    Serial2.print('1');
    wait(500);
  }
  return(0);
}

IMU_Data IMU::read_IMU(){
  bool reading = false;
  std::String receivedValues = "";
  std::string delimiter = ",";
  float numbers[3];
  int numIdx=0;
  while(Serial2.available() > 0){
    int num = Serial2.read();
    if (reading == true){
      if (num != '>'){
        //if new num(character) is not at the end, append string
        receivedValues += (char)num;
      }//end if end
      else{
        reading = false;
        receivedVales += '\0';
      }//done reading
    }
    //check for start character
    else if (num == '<'){
      reading = true;
    }
  }
  size_t pos = 0;
  std::String token;
  while((pos = receivedValues.find(delimiter)) != std::string::npos){
    token = receivedValues.substr(0,pos);
    numbers[numIdx] = std::stof(token);
    numIdx++;
  }
  set_Data(numbers[0], numbers[1], numbers[2]);
}

void IMU::set_Data(float accelU, float accelH, float dir){
  data.accelUp = accelU;
  data.accelHoriz = accelH;
  data.direction = dir;
}

IMU_Data IMU::get_Data(){
  return(data);
}

// }}}

// GPS {{{
#define GPS_DEFAULT_ADDR 0x10
#define GPS_DEFAULT_WAKE 22
#define GPS_DEFAULT_RESETN 23
#define GPS_DEFAULT_INT 24
// Default Constructor, assumes default addr is 0x10
GPS::GPS() {
  set_addr(GPS_DEFAULT_ADDR);
  set_wake(GPS_DEFAULT_WAKE);
  set_reset(GPS_DEFAULT_RESETN);
  set_int(GPS_DEFAULT_INT);
  create_GPS(get_addr());
}

// Constructor, provide address
GPS::GPS(int _addr, int _wake, int _reset, int _int) {
  set_addr(_addr);
  set_wake(_wake);
  set_reset(_reset);
  set_int(_int);
  create_GPS(get_addr());
}

GPS_Data GPS::read_GPS() {
  /* Update current position and then return that position
   *
   * Returns
   * -------
   *  GPS_Data: struct containing altitude, longitude, and latitude
   */
  digitalWrite(get_wake(), HIGH); // Wake GPS
  delay(50);

  while (!digitalRead(get_int())); // Wait for GPS to be ready

  while (myGPS.available()) { // available() returns # of new bytes that can be read
    GPS_Parser.encode(myGPS.read()); // Give data to parser
  }

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
  while (myGPS.begin() && counter < 5) {
    digitalWrite(get_reset(), HIGH);
    serial.println("GPS failed to initialize")
    delay(50);
    counter++;
    if (counter == 5) {
      serial.println("GPS failed 5x, aborting!")
    } // end if
  } // end while
}

// get functions
int GPS::get_addr() {return(addr);}

int GPS::get_wake() {return(wake);}

int GPS::get_reset() {return(reset);}

int GPS::get_int() {return(interact);}

GPS_Data GPS::get_data() {return(data);}

float GPS::get_alt() {return(data.altitude);}

float GPS::get_lat() {return(data.latitude);}

float GPS::get_long() {return(data.longitude);}

int GPS::get_sats() {return(data.satellites);}

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

void GPS::set_alt(float _alt) {
  data.altitude = _alt;
}

void GPS::set_lat(float _lat) {
  data.latitude = _lat;
}

void GPS::set_long(float _long) {
  data.longitude = _long;
}

void GPS::set_sats(int _sats) {
  data.satellites = _sats;
}

// }}}
// Battery Voltage {{{
float check_battery(void)
{
  /* Measurement of the battery health */
  float Battery_Measurement = 5 * ((float)analogRead(Battery))/1024;
  // TODOC: This equation is not obvious at first glance, provide some
  // explanation about what is doing.
  return(100 * (Battery_Divider * Battery_Measurement - Battery_Min) /
         (Battery_Max - Battery_Min));
}
