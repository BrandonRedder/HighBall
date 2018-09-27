
#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>
#include <IridiumSBD.h>
//#include "SparkFun_I2C_GPS_Arduino_Library.h"
#include <Servo.h>

#define IridiumSerial Serial3
#define SLEEP_PIN 4
#define DIAGNOSTICS true // Change this to see diagnostics

//I2CGPS myI2CGPS; //Hook object to the library
// Declare the IridiumSBD object (note SLEEP pin)
IridiumSBD modem(IridiumSerial, SLEEP_PIN);
MS5803 sensor(ADDRESS_HIGH);
Servo TestServo;

//Create variables to store results
float temperature_c, temperature_f;
double pressure_abs, pressure_relative, altitude_delta, pressure_baseline;

// Create Variable to store altitude in (m) for calculations;
double base_altitude = 1655.0; // Altitude of SparkFun's HQ in Boulder, CO. in (m)

int angle = 0;

void setup() {
  // Start the console serial port
  Serial.begin(115200);
  while (!Serial);

  // Start the serial port connected to the satellite modem
  IridiumSerial.begin(19200);

  Serial.println("GTOP Read Example");

  //if (myI2CGPS.begin() == false)
  //{
  //  Serial.println("Module failed to respond. Please check wiring.");
  //  while (1); //Freeze!
  //}
  //Serial.println("GPS module found!");


  //Retrieve calibration constants for conversion math.
  sensor.reset();
  sensor.begin();

  pressure_baseline = sensor.getPressure(ADC_4096);

  TestServo.attach(9);
}

void loop() {
  // Begin satellite modem operation
  Serial.println("Starting modem...");
  int err = modem.begin();
  if (err != ISBD_SUCCESS)
  {
    Serial.print("Begin failed: error ");
    Serial.println(err);
    if (err == ISBD_NO_MODEM_DETECTED)
      Serial.println("No modem detected: check wiring.");
    return;
  }

  // Put modem to sleep
  Serial.println("Putting modem to sleep.");
  err = modem.sleep();
  if (err != ISBD_SUCCESS)
  {
    Serial.print("sleep failed: error ");
    Serial.println(err);
  }

  // Demonstrate that device is asleep
  Serial.println("Trying to send while asleep.");
  err = modem.sendSBDText("This shouldn't work.");
  if (err == ISBD_IS_ASLEEP)
  {
    Serial.println("Couldn't send: device asleep.");
  }
  else
  {
    Serial.print("Send failed for some other reason: error ");
    Serial.println(err);
  }

  //if (myI2CGPS.available()) //available() returns the number of new bytes available from the GPS module
  //{
  //  byte incoming = myI2CGPS.read(); //Read the latest byte from Qwiic GPS
  
  //  if (incoming == '$') Serial.println(); //Break the sentences onto new lines
  //  Serial.write(incoming); //Print this character
  //}

  // Read temperature from the sensor in deg C. This operation takes about
  temperature_c = sensor.getTemperature(CELSIUS, ADC_512);

  // Read temperature from the sensor in deg F. Converting
  // to Fahrenheit is not internal to the sensor.
  // Additional math is done to convert a Celsius reading.
  temperature_f = sensor.getTemperature(FAHRENHEIT, ADC_512);

  // Read pressure from the sensor in mbar.
  pressure_abs = sensor.getPressure(ADC_4096);

  // Let's do something interesting with our data.

  // Convert abs pressure with the help of altitude into relative pressure
  // This is used in Weather stations.
  pressure_relative = sealevel(pressure_abs, base_altitude);

  // Taking our baseline pressure at the beginning we can find an approximate
  // change in altitude based on the differences in pressure.
  altitude_delta = altitude(pressure_abs , pressure_baseline);

  // Report values via UART
  Serial.print("Temperature C = ");
  Serial.println(temperature_c);

  Serial.print("Temperature F = ");
  Serial.println(temperature_f);

  Serial.print("Pressure abs (mbar)= ");
  Serial.println(pressure_abs);

  Serial.print("Pressure relative (mbar)= ");
  Serial.println(pressure_relative);

  Serial.print("Altitude change (m) = ");
  Serial.println(altitude_delta);

  if (angle > 180) {
    angle = 0;
  } else {
    angle = angle + 45;
  }

  TestServo.write(angle);

  Serial.println("Sleeping for 5 seconds.");
  delay(5 * 1000UL);

}

double sealevel(double P, double A)
// Given a pressure P (mbar) taken at a specific altitude (meters),
// return the equivalent pressure (mbar) at sea level.
// This produces pressure readings that can be used for weather measurements.
{
  return (P / pow(1 - (A / 44330.0), 5.255));
}


double altitude(double P, double P0)
// Given a pressure measurement P (mbar) and the pressure at a baseline P0 (mbar),
// return altitude (meters) above baseline.
{
  return (44330.0 * (1 - pow(P / P0, 1 / 5.255)));
}
