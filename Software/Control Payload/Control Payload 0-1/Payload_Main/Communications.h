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
#define PRS_OFFSET 167
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

// Helium Data
#define HEL_MIN 0
#define HEL_STEP 1
#define HEL_OFFSET 137
#define HEL_BITS 10

// Ballast Data
#define BALL_MIN 0
#define BALL_STEP .1
#define BALL_OFFSET 147
#define BALL_BITS 10

// Battery Data
#define BATT_MIN 0
#define BATT_STEP .1
#define BATT_OFFSET 157
#define BATT_BITS 10

// Time Data
#define TIME_MIN 0
#define TIME_STEP 1
#define TIME_OFFSET 179
#define TIME_BITS 17

// State Data
#define STATE_OFFSET 196
#define STATE_BITS 3

// Control Mode Data
#define CTRL_OFFSET 199
#define CTRL_BITS 2

// Emergency Data
#define EMRG_OFFSET 201
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
#define REC_ALT_BUF_LENGTH 10

// rec_lat_degree
#define REC_LAT_MIN -16
#define REC_LAT_STEP 1
#define REC_LAT_OFFSET 23 
#define REC_LAT_LENGTH 5

// rec_lat_deg_min
#define REC_LAT_MIN_MIN -10
#define REC_LAT_MIN_STEP .0001
#define REC_LAT_MIN_OFFSET 28
#define REC_LAT_MIN_LENGTH 18

// rec_long_deg
#define REC_LONG_MIN -16
#define REC_LONG_STEP 1
#define REC_LONG_OFFSET 46
#define REC_LONG_LENGTH 5

// rec_long_deg_min 
#define REC_LONG_MIN_MIN -10
#define REC_LONG_MIN_STEP .0001
#define REC_LONG_MIN_OFFSET 51
#define REC_LONG_MIN_LENGTH 18

// cutdown
#define REC_CUT_MIN 0
#define REC_CUT_STEP 1
#define REC_CUT_OFFSET 69
#define REC_CUT_LENGTH 2

// rec_update_rate
#define REC_UPD_MIN 0
#define REC_UPD_STEP 30
#define REC_UPD_OFFSET 71
#define REC_UPD_LENGTH 9

// rec_hel_alpha
#define REC_HEL_A_MIN 0
#define REC_HEL_A_STEP .01
#define REC_HEL_A_OFFSET 80
#define REC_HEL_A_LENGTH 10

// rec_hel_beta
#define REC_HEL_B_MIN 0
#define REC_HEL_B_STEP .01
#define REC_HEL_B_OFFSET 90
#define REC_HEL_B_LENGTH 10

// rec_hel_gamma
#define REC_HEL_G_MIN 0 
#define REC_HEL_G_STEP .01
#define REC_HEL_G_OFFSET 100
#define REC_HEL_G_LENGTH 10

// rec_bal_alpha
#define REC_BAL_A_MIN 0
#define REC_BAL_A_STEP .01
#define REC_BAL_A_OFFSET 110
#define REC_BAL_A_LENGTH 10

// rec_bal_beta 
#define REC_BAL_B_MIN 0
#define REC_BAL_B_STEP .01
#define REC_BAL_B_OFFSET 120
#define REC_BAL_B_LENGTH 10

// rec_bal_gamma
#define REC_BAL_G_MIN 0
#define REC_BAL_G_STEP .01
#define REC_BAL_G_OFFSET 130
#define REC_BAL_G_LENGTH 10

// rec_length_vent
#define REC_VENT_TIME_MIN 0
#define REC_VENT_TIME_STEP .1
#define REC_VENT_TIME_OFFSET 140
#define REC_VENT_TIME_LENGTH 10

// rec_vent_inc
#define REC_VENT_INC_MIN 0 
#define REC_VENT_INC_STEP 1
#define REC_VENT_INC_OFFSET 150 
#define REC_VENT_INC_LENGTH 7

// rec_ballast_inc
#define REC_BAL_INC_MIN 0
#define REC_BAL_INC_STEP 1
#define REC_BAL_INC_OFFSET 157
#define REC_BAL_INC_LENGTH 7

// rec_temp_setpoint
#define REC_TEMP_S_MIN -70
#define REC_TEMP_S_STEP .5
#define REC_TEMP_S_OFFSET 164
#define REC_TEMP_S_LENGTH 7

// rec_temp_P
#define REC_TEMP_P_MIN 0
#define REC_TEMP_P_STEP .001
#define REC_TEMP_P_OFFSET 171
#define REC_TEMP_P_LENGTH 14

// rec_temp_I
#define REC_TEMP_I_MIN 0
#define REC_TEMP_I_STEP .001
#define REC_TEMP_I_OFFSET 185
#define REC_TEMP_I_LENGTH 14

// rec_temp_D
#define REC_TEMP_D_MIN 0
#define REC_TEMP_D_STEP .001
#define REC_TEMP_D_OFFSET 199
#define REC_TEMP_D_LENGTH 14

// rec_control_mode
#define REC_CTRL_MIN 0
#define REC_CTRL_STEP 1
#define REC_CTRL_OFFSET 213
#define REC_CTRL_LENGTH 2

// rec_manual_adjust
#define REC_MAN_ADJ_MIN 0
#define REC_MAN_ADJ_STEP 1
#define REC_MAN_ADJ_OFFSET 215
#define REC_MAN_ADJ_LENGTH 1

// rec_manual_select
#define REC_MAN_SEL_MIN 0 
#define REC_MAN_SEL_STEP 1
#define REC_MAN_SEL_OFFSET 216
#define REC_MAN_SEL_LENGTH 1

// rec_manual_amount
#define REC_MAN_AMT_MIN 0
#define REC_MAN_AMT_STEP 1
#define REC_MAN_AMT_OFFSET 217 
#define REC_MAN_AMT_LENGTH 10

// Structures
struct Incoming_Data
{
	// Position Data 
	float altitude;
	float altitude_buffer;
	float lat_degree;
	float lat_deg_min;
	float long_deg;
	float long_deg_min;
	// Cutdown
	bool cutdown;
	// Protocol
	int update_rate;
	// Control Mode
	int control_mode;
	// Advanced Control
	float hel_alpha;
	float hel_beta;
	float hel_gamma;
	float bal_alpha;
	float bal_beta;
	float bal_gamma;
	// Basic Control
	float length_vent;
	float vent_inc;
	float ballast_inc;
	// Manual Control
	bool manual_adjust;
	bool manual_select;
	int manual_amount;
	// Temperature Control
	float temp_setpoint;
	float temp_P;
	float temp_I;
	float temp_D;
};

struct Outgoing_Data
{
	// Temperature Data
	float temperature;
	// Pressure Data
	float pressure;
	// Acceleration Data
	float acc_magnitude;
	float acc_theta;
	float acc_phi;
	// GPS Data
	float altitude;
	float latitude;
	float longitude;
	// Battery Data
	float BatteryPercentage;
	// Velovity Data
	float vel_magnitude;
	float vel_theta;
	float vel_phi;
	// Ballast Data
	int ballast;
	// Helium Data
	int helium;
	// Time Data
	int time;
	// State Data
	int state;
	// Control Mode Data
	int control_mode;
	// Emergency Data
	int emergency;
};

int status = 10;

//Functions
void setup_Communications(void);
void call_iridium(int);
void send_message(int);
void encode_message (struct Outgoing_Data *);
void decode_message (struct Incoming_Data *);
int convert_int (int data, float min, float step);
int convert_float (float data, float min, float step);
int convert_double (double data, float min, float step);
void encode_data (int data, int bit_offset, int bit_length);
float decode_data (float min, float step, int offset, int bits);
bool ISBDCallback(void);
void ISBDConsoleCallback(IridiumSBD *device, char c);
void ISBDDiagsCallback(IridiumSBD *device, char c);


#endif /* ifndef COMMUNICATIONS_H */
