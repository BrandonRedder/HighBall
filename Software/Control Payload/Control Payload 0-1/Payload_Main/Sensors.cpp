#include "Sensors.h"


//Functions
//Main Setup Function
void setup_Sensors(void)
{
  setup_PIT_01();
  setup_PIT_02();
  setup_IMU_01();
  setup_GPS_01();
  check_battery();
}

//Temperature Sensor
//Messure Temperature in Kelvin
float read_TIT_01(void)
{
  float voltage = 5 * ((float)analogRead(TIT_01))/1024.0; //0 to 1023 and 0 to 5V
  return TIT_01_CAL * voltage * 100; //1K per 10mV
}

//Pressure Sensors
MS5803 PIT_01(PIT_01_ADR);
MS5803 PIT_02(PIT_02_ADR);
double PIT_01_Pressure_Baseline;
double PIT_02_Pressure_Baseline;

void setup_PIT_01(void)
{
  PIT_01.reset();
  PIT_01.begin();
  PIT_01_Pressure_Baseline = PIT_01.getPressure(ADC_4096);
}

void setup_PIT_02(void)
{
  PIT_02.reset();
  PIT_02.begin();
  PIT_02_Pressure_Baseline = PIT_02.getPressure(ADC_4096);
}

void read_PIT_01(void)
{
  float pressure = PIT_01.getPressure(ADC_4096);
  return pressure;
}

float read_PIT_02(void)
{
  float pressure = PIT_02.getPressure(ADC_4096);
  return pressure;
}

//IMU
void setup_IMU_01(void)
{
  
}

void read_IMU_01(void)
{
  
}

//GPS
I2CGPS myI2CGPS;
TinyGPSPlus GPS_Parser;
double GPS_Altitude;
double GPS_Lat;
double GPS_Long;
int GPS_Sat; //Number of Satellites

void setup_GPS_01(void)
{
  #define GPS_01_ADR 0x10
  pinMode(GPS_01_ResetN, OUTPUT);
  pinMode(GPS_01_Wake, OUTPUT);
  pinMode(GPS_01_INT, INPUT);
  
  digitalWrite(GPS_01_ResetN, HIGH);
  digitalWrite(GPS_01_Wake, HIGH);
  
  myI2CGPS.begin()

}

void read_GPS_01(void)
{
  digitalWrite(GPS_01_Wake, HIGH); //Wake-up GPS
  delay(50);
  
  while(!digitalRead(GPS_01_INT)); //Wait for GPS to be ready
  
  while (myI2CGPS.available()) //available() returns the number of new bytes available from the GPS module
  {
    GPS_Parser.encode(myI2CGPS.read()); //Feed the GPS parser
  }

  GPS_Altitude = GPS_Parser.altitude.feet();
  GPS_Lat = GPS_Parser.location.lat();
  GPS_Long = GPS_Parser.location.lng();
  GPS_Sat = GPS_Parser.satellites.value();

  digitalWrite(GPS_01_Wake, LOW); //Put GPS into Low Power Mode
}

//Battery Voltage
float Battery_Percentage;
void check_battery(void)
{
  float Battery_Measurement = 5 * ((float)analogRead(Battery))/1024;
  Battery_Percentage = 100 * (Battery_Divider * Battery_Measurement - Battery_Min) / (Battery_Max - Battery_Min);
}

