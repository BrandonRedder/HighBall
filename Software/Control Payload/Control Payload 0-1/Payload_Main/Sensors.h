#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>
#include <SparkFun_I2C_GPS_Arduino_Library.h>
#include <TinyGPS++.h>
//#include <SparkFunMPU9250-DMP.h>

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
};

// Pressure Sensor
class pressure_sensor {
  public:
    pressure_sensor();
    pressure_sensor(ms5803_addr);
    //pressure_sensor(MS5803&);
    float read_pressure();
    float find_altitude(float);

    void create_sensor(int);
    void initialize_sensor();

    ms5803_addr get_addr();
    //MS5803& get_sensor();
    float get_baseline();
    void set_addr(ms5803_addr);
    void set_baseline(float);

    MS5803 sensor;


  private:
    int addr;
    float baseline;
};

//IMU
struct IMU_Data{
  float accelUp;
  float accelHoriz;
  float direction;
};

class IMU {
  public:
      IMU();
      int comms_Test();
      IMU_Data read_IMU();
      IMU_Data get_Data();
      void set_Data(float, float, float);
      void initialize_IMU();
    private:
      IMU_Data data;
};
// GPS
struct GPS_Data {
  double altitude;
  double latitude;
  double longitude;
  uint32_t satellites;
};

class GPS {
  public:
    GPS();
    GPS(int, int, int, int);
    GPS_Data read_GPS();

    void create_GPS();
    void initialize_GPS();

    int get_addr();
    int get_wake();
    int get_reset();
    int get_int();
    GPS_Data get_data();
    double get_alt();
    double get_lat();
    double get_long();
    uint32_t get_sats();
    void set_addr(int);
    void set_wake(int);
    void set_reset(int);
    void set_int(int);
    void set_data(GPS_Data);
    void set_alt(double);
    void set_lat(double);
    void set_long(double);
    void set_sats(uint32_t);

  private:
    int addr;
    int wake;
    int reset;
    int interact;
    I2CGPS myGPS;
    TinyGPSPlus GPS_Parser;
    GPS_Data data;
};

// Battery
float check_battery();
/*
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
*/
#endif /* ifndef SENSORS_H */
