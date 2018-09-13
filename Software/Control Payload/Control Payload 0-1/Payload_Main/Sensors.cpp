#include "Sensors.h"

// Temperature Sensor {{{
// Default Constructor, assumes 100 cal and pin A2
temperature_sensor::temperature_sensor() {
  set_cal(100.0); // 10 mV <-> 1 Kelvin
  set_pin(A2);
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
// Default constructor, assumes address=0x76
pressure_sensor::pressure_sensor() {
  set_addr(0x76);
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






//Pressure Sensors
MS5803 PIT_01(PIT_01_ADR);
MS5803 PIT_02(PIT_02_ADR);
double PIT_01_Pressure_Baseline; // Consider Switching to float
double PIT_02_Pressure_Baseline;
// FIXME: These will not be accessible outside this file
//        They should be defined the Sensors.h file

void setup_PIT_01(void)
{
  /* Perform the setup routine for pressure sensor 1 */
  PIT_01.reset();
  PIT_01.begin();
  // FIXME: There should be a way to check that the sensor started correctly
  PIT_01_Pressure_Baseline = PIT_01.getPressure(ADC_4096);
  // FIXME: Baselines are never used, can't be used outside this file as is
}

void setup_PIT_02(void)
{
  /* Perform the setup routine for pressure sensor 2 */
  PIT_02.reset();
  PIT_02.begin();
  // FIXME: There should be a way to check that the sensor started correctly
  PIT_02_Pressure_Baseline = PIT_02.getPressure(ADC_4096);
}

float read_PIT_01(void)
{
  /* Read the current pressure from Presure sensor 1
  Returns
  -------
  float: current pressure measured from Presure sensor 1
  */
  float pressure = PIT_01.getPressure(ADC_4096);
  return pressure;
}

float read_PIT_02(void)
{
  /* Read the current pressure from Presure sensor 2
  Returns
  -------
  float: current pressure measured from Presure sensor 2
  */
  float pressure = PIT_02.getPressure(ADC_4096);
  return pressure;
}

//GPS
I2CGPS myI2CGPS;
TinyGPSPlus GPS_Parser;
double GPS_Altitude;
double GPS_Lat;
double GPS_Long;
int GPS_Sat; //Number of Satellites
// FIXME: These will not be accessible outside this file
//        They should be defined the Sensors.h file
//        This is the same as for Pressure
// FIXME: At a minimum, these values should be grouped into a struct to help
//        readability

void setup_GPS_01(void)
{
  /* Initialize the GPS Module */
  #define GPS_01_ADR 0x10 // FIXME: Duplicated in .h file
  pinMode(GPS_01_ResetN, OUTPUT);
  pinMode(GPS_01_Wake, OUTPUT);
  pinMode(GPS_01_INT, INPUT);

  digitalWrite(GPS_01_ResetN, HIGH);
  digitalWrite(GPS_01_Wake, HIGH);

  myI2CGPS.begin()

}

void read_GPS_01(void)
{
  /* Read current position using GPS Module */
  digitalWrite(GPS_01_Wake, HIGH); //Wake-up GPS
  delay(50);

  while(!digitalRead(GPS_01_INT)); //Wait for GPS to be ready

  while (myI2CGPS.available()) //available() returns the number of new bytes available from the GPS module
  {
    GPS_Parser.encode(myI2CGPS.read()); //Feed the GPS parser
  }

  // FIXME: Return a struct instead of altering global variables
  GPS_Altitude = GPS_Parser.altitude.feet();
  GPS_Lat = GPS_Parser.location.lat();
  GPS_Long = GPS_Parser.location.lng();
  GPS_Sat = GPS_Parser.satellites.value();

  digitalWrite(GPS_01_Wake, LOW); //Put GPS into Low Power Mode
}

//Battery Voltage
float Battery_Percentage;
// FIXME: This will not be accessible outside this file
//        It should be defined the Sensors.h file
void check_battery(void)
{
  /* Measurement of the battery health */
  float Battery_Measurement = 5 * ((float)analogRead(Battery))/1024;
  // TODOC: This equation is not obvious at first glance, provide some
  // explanation about what is doing.
  Battery_Percentage = 100 * (Battery_Divider * Battery_Measurement - Battery_Min) / (Battery_Max - Battery_Min);
}
