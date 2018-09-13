#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>
#include <SparkFun_I2C_GPS_Arduino_Library.h>
#include <TinyGPS++.h>
#include <SparkFunMPU9250-DMP.h>

//Temperature Sensor
class temperature_sensor {
  public:
    temperature_sensor();
    temperature_sensor(float, int);
    float read_temp();

    float get_cal();
    int get_pin();
    void set_cal(float);
    void set_pin(int);

  private:
    float calibration;
    int pin;
}

//Constants
// TODOC: These should be explained a bit more
//Temperature Sensor
//Messure Temperature in Kelvin
#define TIT_01 A2 // What is A2? should it be hex?
#define TIT_01_CAL 1.00 // (V_T0 / T0)

//Pressure Sensors
#define PIT_01_ADR 0x76
#define PIT_02_ADR 0x77

//IMU
#define IMU_01_TX 16
#define IMU_01_RX 17

//GPS
#define GPS_01_Wake 22
#define GPS_01_ResetN 23
#define GPS_01_INT
#define GPS_01_ADR 0x10

//Battery Voltage
#define Battery A3
#define Battery_Divider 1.15385
#define Battery_Max 12.6
#define Battery_Min 8.25






float read_TIT_01(void);

//Pressure Sensors
void setup_PIT_01(void);
void setup_PIT_02(void);
float read_PIT_01(void);
float read_PIT_02(void);

//IMU
struct IMU_OUTPUT{
  /* Contains x,y,z magnetometer data, and quaternian acceleration data */
  float xMag = 0;
  float yMag = 0;
  float zMag = 0;
  float q0 = 0;
  float q1 = 0;
  float q2 = 0;
  float q3 = 0;
};
void setup_IMU_01(void);
IMU_OUTPUT read_IMU_01(void);

//GPS
void setup_GPS_01(void);
void read_GPS_01(void);

//Battery Voltage
void check_battery(void);

#endif /* ifndef SENSORS_H */
