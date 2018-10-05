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
};

// Pressure Sensor
class pressure_sensor {
  public:
    pressure_sensor();
    pressure_sensor(int);
    pressure_sensor(MS5803&);
    float read_pressure();
    float find_altitude();

    void create_sensor(int);
    void initialize_sensor();

    int get_addr();
    MS5803& get_sensor();
    float get_baseline();
    void set_addr(int);
    void set_baseline(float);

  private:
    int addr;
    MS5803 sensor;
    float baseline;
};

// GPS
struct GPS_Data {
  float altitude;
  float latitude;
  float longitude;
  int satellites;
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
    float get_alt();
    float get_lat();
    float get_long();
    int get_sats();
    void set_addr(int);
    void set_wake(int);
    void set_reset(int);
    void set_int(int);
    void set_data(GPS_Data);
    void set_alt(float);
    void set_lat(float);
    void set_long(float);
    void set_sats(int);

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

#endif /* ifndef SENSORS_H */
