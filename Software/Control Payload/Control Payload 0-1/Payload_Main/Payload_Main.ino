#include "Sensors.h"
#include "Actuators.h"
#include "Communications.h"
#include "Control.h"
#include "State_Machine.h"

#define PRESSURE_01_ADR 0x76
#define PRESSURE_02_ADR 0x77
temperature_sensor temp;
pressure_sensor pressure1(PRESSURE_01_ADR);
pressure_sensor pressure2(PRESSURE_02_ADR);
IMU imu;
GPS gps;

void setup()
{
}

#define NO_ACTION 0
#define DROP_BALLAST 1
#define RELEASE_HELIUM 2

void loop()
{
  float temp = temp.read_temp();
  float altitude1 = pressure1.find_altitude();
  float altitude2 = pressure2.find_altitude();
  IMU_Data imu_data = imu.read_imu();
  GPS_Data gps_data = gps.read_GPS();
  float battery_level = check_battery();
}
