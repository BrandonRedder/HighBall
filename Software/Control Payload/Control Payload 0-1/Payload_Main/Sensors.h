#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>

//Constants
//Temperature Sensor
//Messure Temperature in Kelvin
#define TIT_01 A2
#define TIT_01_CAL 1.00

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

//Functions
//Temperature Sensor
float read_TIT_01(void);

//Pressure Sensors
void setup_PIT_01(void);
void setup_PIT_02(void);
float read_PIT_01(void);
float read_PIT_02(void);

//IMU
void setup_IMU_01(void);
void read_IMU_01(void);

//GPS
void setup_GPS_01(void);
void read_GPS_01(void);
