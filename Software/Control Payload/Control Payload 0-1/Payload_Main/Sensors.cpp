#include "Sensors.h"

//Functions
//Main Setup Function
void setup_Sensors(void)
{
  /* Initializes 2 Pressure sensors, IMU, GPS, and Checks Battery level */
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
  /* Reads the analog output level and converts to temperature data
  Returns
  -------
  float: Temperature measured by the Temperature Sensor
  */
  float voltage = 5 * ((float)analogRead(TIT_01))/1024.0; //0 to 1023 and 0 to 5V
  // FIXME: Not sure if units agree on this return compared with TIT_01_CAL
  // FIXME: Also, the 100 should probably be accounted for in the calibration
  return TIT_01_CAL * voltage * 100; //1K per 10mV
}

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

//IMU
MPU9250_DMP imu;
void setup_IMU_01(void)
{
  /*
  Serial2.begin(115200); //set baud rate

  // Check communication with IMU
  if (imu.begin() != 0)
  {
    while(imu.begin() != 0)
    {
      Serial2.println("Unable to connect with IMU.");
      delay(5000);
    }//end while display
  }//end if error in connection

  //enable 6-axis quaternion output
  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL, 50);//check sampling rate

  //enable XYZ magnetometer for absolute orientation determination
  imu.setSensors(INV_XYZ_COMPASS);
  imu.setCompassSampleRate(50);//check sampling rate
  */
}//end setup_IMU_01

IMU_OUTPUT read_IMU_01(void)
{
  /*
  IMU_OUTPUT updateValues;
  if ( imu.fifoAvailable() > 0 )//if there is new data
  {
    if (imu.dmpUpdateFifo() == 0)//update data
    {
      //update orientation
      imu.update(UPDATE_COMPASS);

      updateValues.xMag = imu.calcMag(imu.mx);
      updateValues.yMag = imu.calcMag(imu.my);
      updateValues.xMag = imu.calcMag(imu.mz);
      updateValues.q0 = imu.qw;
      updateValues.q1 = imu.qx;
      updateValues.q2 = imu.qy;
      updateValues.q3 = imu.qz;
      }
  }
  return updateValues;
  */
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
