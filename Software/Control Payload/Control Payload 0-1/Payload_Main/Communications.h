#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "Sensors.h"
#include "State_Machine.h"
#include "IridiumSBD.h"
#include "math.h"

//Constants
#define IridiumSerial Serial1
#define WiredSerial Serial
#define SLEEP_PIN 25
#define RING_PIN 3
#define DIAGNOSTICS true

// Alarm Conditions
#define MAX_ACCEL 12
#define MIN_POWER_PERCENT 5
#define MIN_BALLAST_PERCENT 2
#define MIN_INT_TEMP -50

// Temperature Data
#define TEMP_MIN -88
#define TEMP_STEP .25
#define TEMP_OFFSET 128
#define TEMP_BITS 9
// Pressure Data
#define PRS_MIN 0
#define PRS_STEP .25
#define PRS_OFFSET 157
#define PRS_BITS 12
// IMU Data
#define ACC_MAG_MIN 0
#define ACC_MAG_STEP .001
#define ACC_MAG_OFFSET 95
#define ACC_MAG_BITS 14

#define ACC_THETA_MIN 0
#define ACC_THETA_STEP 0.0125663706
#define ACC_THETA_OFFSET 109
#define ACC_THETA_BITS 10

#define ACC_PHI_MIN 0
#define ACC_PHI_STEP 0.00628318531
#define ACC_PHI_OFFSET 119
#define ACC_PHI_BITS 9
// GPS Data
#define ALT_MIN -5000
#define ALT_STEP 3
#define ALT_OFFSET 0
#define ALT_BITS 14

#define LAT_MIN -100
#define LAT_STEP .0001
#define LAT_OFFSET 14
#define LAT_BITS 23

#define LONGITUDE_MIN 18
#define LONG_STEP .0001
#define LONG_OFFSET 37
#define LONG_BITS 23
// Battery Data
#define BATT_MIN 0
#define BATT_STEP .1
#define BATT_OFFSET 147
#define BATT_BITS 10
// Velovity Data
#define VEL_MAG_MIN 0
#define VEL_MAG_STEP 0.001
#define VEL_MAG_OFFSET 60
#define VEL_MAG_BITS 16

#define VEL_THETA_MIN 0
#define VEL_THETA_STEP 0.0125663706
#define VEL_THETA_OFFSET 76
#define VEL_THETA_BITS 10

#define VEL_PHI_MIN 0
#define VEL_PHI_STEP 0.00628318531
#define VEL_PHI_OFFSET 86
#define VEL_PHI_BITS 9
// Ballast Data
#define BALL_MIN 0
#define BALL_STEP .1
#define BALL_OFFSET 137
#define BALL_BITS 10

int status = 10;


//Functions
void setup_Communications(void);
void call_iridium(void);
void send_message(void);
void check_status (void);
void encode_all (void);
void decode_message (void);
int convert_int (int data, float min, float step);
int convert_float (float data, float min, float step);
int convert_double (double data, float min, float step);
void encode_data (int data, int bit_offset, int bit_length);
float decode_data (float min, float step, int offset, int bits);
void convert_IMU_data(void);
bool ISBDCallback(void);
void ISBDConsoleCallback(IridiumSBD *device, char c);
void ISBDDiagsCallback(IridiumSBD *device, char c);


#endif /* ifndef COMMUNICATIONS_H */
