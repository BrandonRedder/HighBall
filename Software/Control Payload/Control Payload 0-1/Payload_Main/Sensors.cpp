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

float read_PIT_01(void)
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
void setup_GPS_01(void)
{
  
}
void read_GPS_01(void)
{
  
}

//Battery Voltage
float Battery_Percentage;
void check_battery(void)
{
  float Battery_Measurement = 5 * ((float)analogRead(Battery))/1024;
  Battery_Percentage = 100 * (Battery_Divider * Battery_Measurement - Battery_Min) / (Battery_Max - Battery_Min);
}

