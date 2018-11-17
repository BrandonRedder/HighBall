#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "IridiumSBD.h"
#include "math.h"

//Constants
#define IridiumSerial Serial1
#define WiredSerial Serial
#define SLEEP_PIN 25
#define RING_PIN 3
#define DIAGNOSTICS true

#define Testing true

// Outgoing Data
// Temperature Data
#define TEMP_MIN -88
#define TEMP_STEP .25
#define TEMP_OFFSET 0
#define TEMP_BITS 9
// Pressure Data
#define PRS_MIN 0
#define PRS_STEP .25
#define PRS_OFFSET 9
#define PRS_BITS 12
// IMU Data
#define ACC_VER_MIN 0
#define ACC_VER_STEP .001
#define ACC_VER_OFFSET 21
#define ACC_VER_BITS 14

#define ACC_HOR_MIN 0
#define ACC_HOR_STEP 0.001
#define ACC_HOR_OFFSET 35
#define ACC_HOR_BITS 14

#define ACC_THETA_MIN 0
#define ACC_THETA_STEP 0.00628318531
#define ACC_THETA_OFFSET 49
#define ACC_THETA_BITS 10
// GPS Data
#define ALT_MIN 0
#define ALT_STEP 3
#define ALT_OFFSET 59
#define ALT_BITS 14

#define LAT_MIN 18
#define LAT_STEP .000005
#define LAT_OFFSET 73
#define LAT_BITS 23

#define LONGITUDE_MIN -100
#define LONG_STEP .000005
#define LONG_OFFSET 96
#define LONG_BITS 23

// Velovity Data
#define VEL_MAG_MIN 0
#define VEL_MAG_STEP 0.0005
#define VEL_MAG_OFFSET 119
#define VEL_MAG_BITS 16

// Helium Data
#define HEL_MIN 0
#define HEL_STEP 0.1
#define HEL_OFFSET 135
#define HEL_BITS 10

// Ballast Data
#define BALL_MIN 0
#define BALL_STEP 0.1
#define BALL_OFFSET 145
#define BALL_BITS 10

// Time Data
#define TIME_MIN 0
#define TIME_STEP 1
#define TIME_OFFSET 155
#define TIME_BITS 17

// State Data
#define STATE_OFFSET 172
#define STATE_BITS 3

// Control Mode Data
#define CTRL_OFFSET 175
#define CTRL_BITS 2

// Emergency Data
#define EMRG_OFFSET 177
#define EMRG_BITS 2

// Recieved Data
//rec_altitude
#define REC_ALT_MIN 0
#define REC_ALT_STEP 5
#define REC_ALT_OFFSET 0
#define REC_ALT_LENGTH 13

// rec_altitude_buffer
#define REC_ALT_BUF_MIN 0
#define REC_ALT_BUF_STEP 5
#define REC_ALT_BUF_OFFSET 13
#define REC_ALT_BUF_LENGTH 11

// rec_lat_degree
#define REC_LAT_MIN 18
#define REC_LAT_STEP 0.000005
#define REC_LAT_OFFSET 24
#define REC_LAT_LENGTH 23

// rec_long_deg
#define REC_LONG_MIN -100
#define REC_LONG_STEP 0.000005
#define REC_LONG_OFFSET 47
#define REC_LONG_LENGTH 23

// cutdown
#define REC_CUT_MIN 0
#define REC_CUT_STEP 1
#define REC_CUT_OFFSET 70
#define REC_CUT_LENGTH 2

// rec_update_rate
#define REC_UPD_MIN 0
#define REC_UPD_STEP 1
#define REC_UPD_OFFSET 72
#define REC_UPD_LENGTH 10

// rec_hel_alpha
#define REC_HEL_A_MIN 0
#define REC_HEL_A_STEP .001
#define REC_HEL_A_OFFSET 82
#define REC_HEL_A_LENGTH 10

// rec_hel_beta
#define REC_HEL_B_MIN 0
#define REC_HEL_B_STEP .1
#define REC_HEL_B_OFFSET 92
#define REC_HEL_B_LENGTH 10

// rec_hel_gamma
#define REC_HEL_G_MIN 0 
#define REC_HEL_G_STEP .01
#define REC_HEL_G_OFFSET 102
#define REC_HEL_G_LENGTH 10

// rec_bal_alpha
#define REC_BAL_A_MIN 0
#define REC_BAL_A_STEP .001
#define REC_BAL_A_OFFSET 112
#define REC_BAL_A_LENGTH 10

// rec_bal_beta 
#define REC_BAL_B_MIN 0
#define REC_BAL_B_STEP .1
#define REC_BAL_B_OFFSET 122
#define REC_BAL_B_LENGTH 10

// rec_bal_gamma
#define REC_BAL_G_MIN 0
#define REC_BAL_G_STEP .01
#define REC_BAL_G_OFFSET 132
#define REC_BAL_G_LENGTH 10

// rec_temp_setpoint
#define REC_TEMP_MIN 0
#define REC_TEMP_STEP 1
#define REC_TEMP_OFFSET 142
#define REC_TEMP_LENGTH 8

// rec_control_mode
#define REC_CTRL_MIN 0
#define REC_CTRL_STEP 1
#define REC_CTRL_OFFSET 150
#define REC_CTRL_LENGTH 2

// rec_manual_adjust
#define REC_MAN_ADJ_MIN 0
#define REC_MAN_ADJ_STEP 1
#define REC_MAN_ADJ_OFFSET 152
#define REC_MAN_ADJ_LENGTH 1

// rec_manual_select
#define REC_MAN_SEL_MIN 0 
#define REC_MAN_SEL_STEP 1
#define REC_MAN_SEL_OFFSET 153
#define REC_MAN_SEL_LENGTH 1

// rec_manual_amount
#define REC_MAN_AMT_MIN 0
#define REC_MAN_AMT_STEP 1
#define REC_MAN_AMT_OFFSET 154 
#define REC_MAN_AMT_LENGTH 10

// max velocity
#define REC_VEL_MIN 0
#define REC_VEL_STEP 0.01
#define REC_VEL_OFFSET 164 
#define REC_VEL_LENGTH 10

// Structures
struct Incoming_Data
{
	// Position Data 
	float altitude;
	float altitude_buffer;
	float lat_deg;
	float long_deg;
	// Cutdown
	bool cutdown;
	// Protocol
	int update_rate;
	// Advanced Control
	float hel_alpha;
	float hel_beta;
	float hel_gamma;
	float bal_alpha;
	float bal_beta;
	float bal_gamma;
  float max_velocity;
	// Temperature Control
	float temp;
	// Control Mode
	int control_mode;
	// Manual Control
	bool manual_adjust;
	bool manual_select;
	int manual_amount;
};

struct Outgoing_Data
{
	// Temperature Data
	float temperature;
	// Pressure Data
	float pressure;
	// Acceleration Data
	float acc_ver;
	float acc_hor;
	float acc_theta;
	// GPS Data
	float altitude;
	float latitude;
	float longitude;
	// Velovity Data
	float vel;
	// Helium Data
	int helium;
	// Ballast Data
	int ballast;
	// Time Data
	int time;
	// State Data
	int state;
	// Control Mode Data
	int control_mode;
	// Emergency Data
	int emergency;
};

//Functions
void setup_Communications(void);
bool call_iridium(int);
void send_message(int);
void encode_message (Outgoing_Data*);
bool decode_message (Incoming_Data*);
int convert_int (int, float, float);
long convert_double (double, float, float);
int convert_float (float, float, float);
void encode_data (int, int, int);
float decode_data (float, float, int, int);
bool ISBDCallback(void);
void ISBDConsoleCallback(IridiumSBD, char);
void ISBDDiagsCallback(IridiumSBD, char);


#endif /* ifndef COMMUNICATIONS_H */
