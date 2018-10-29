#include "Communications.h"

//IridiumSBD modem(IridiumSerial, SLEEP_PIN, RING_PIN);
//
//void setup_Communications(void)
//{
//  // Set LED to output for display
//  pinMode(13, OUTPUT);
//
//  // Start the serial ports
//  IridiumSerial.begin(19200);
//
//  // Setup the Iridium modem
//  modem.setPowerProfile(IridiumSBD::DEFAULT_POWER_PROFILE); // high current setting for battery power
//  int start = modem.begin(); // start the modem
//  int signalQuality = -1;
//  int err = modem.getSignalQuality(signalQuality); // get signal quality
//  if (start != ISBD_SUCCESS) {
//	  if (status == 10) {
//      WiredSerial.print("Start Iridium failed: error ");
//      WiredSerial.println(start);
//    }
//  }
//  if (err != ISBD_SUCCESS) {
//	  if (status == 10) {
//      WiredSerial.print("Get Signal quality failed: error ");
//      WiredSerial.println(err);
//	  }
//  } else {
//	  if (status == 10) {
//      WiredSerial.print("Signal Quality: ");
//      WiredSerial.println(signalQuality);
//    }
//  }  
//}


// Set timer between messages and create message sent bit
bool messageSent = true;

// 3 Latched alert bits
bool alt_alert = false;
bool temp_alert = false;
bool fall_alert = false;
bool emergency = false;

// 30 Byte incoming and outgoing message buffer
uint8_t buffer[30] = {62,129,145,22,26,130,12,53,2,2,75,3,32,200,50,12,131,32,200,80,167,128,250,3,232,15,163,129,64,0};;

// Call Iridium module from state machine 
//void call_iridium(int status) {
//  // Check if the modulel is asleep/ responds
//  int err = modem.sleep();
//  if (err != ISBD_SUCCESS && err != ISBD_IS_ASLEEP) {
//	if (status == 10) {
//    WiredSerial.print("Sleep failed: error ");
//    WiredSerial.println(err);
//    }
//  }
//  
//  uint32_t t = millis();
//  uint32_t dt = timer - t;
//  if (emergency || dt > Message_Rate*1000 || !messageSent) {
//    if (messageSent) {
//      timer = millis();
//      if (emergency) {
//	      emergency = false;
//      }
//    }
//    send_message(status);
//  }
//}

//void send_message(int status) {
//  size_t bufferSize = sizeof(buffer);
//	
//  int err;
//  err = modem.sendReceiveSBDBinary(buffer, 30, buffer, bufferSize);
// 
//  if (err != ISBD_SUCCESS)
//  {
//    messageSent = false;
//    if (status == 10) {
//      WiredSerial.print("Message failed to send");
//      WiredSerial.println(err);
//    }
//  }
//  else // success!
//  {
//    messageSent = true;
//    if (status == 10) {
//      WiredSerial.print("Inbound buffer size is ");
//      WiredSerial.println(bufferSize);
//      for (int i=0; i<bufferSize; ++i)
//      {
//        WiredSerial.print(buffer[i], HEX);
//        if (isprint(buffer[i]))
//        {
//          WiredSerial.print("(");
//          WiredSerial.write(buffer[i]);
//          WiredSerial.print(")");
//        }
//        WiredSerial.print(" ");
//      }
//    }
//  }
//}


void encode_message (struct Outgoing_Data *data) {
  // Temperature Data 
  int new_temperature = convert_float((*data).temperature, TEMP_MIN, TEMP_STEP);
  encode_data (new_temperature, TEMP_OFFSET, TEMP_BITS);
  // Pressure Data
  int new_pressure = convert_float((*data).pressure, PRS_MIN, PRS_STEP);
  encode_data (new_pressure, PRS_OFFSET, PRS_BITS);
  // IMU Data
  int new_acc_magnitude = convert_float((*data).acc_magnitude, ACC_MAG_MIN, ACC_MAG_STEP);
//  Serial.println("value");
//  Serial.println(new_acc_magnitude);
  encode_data (new_acc_magnitude, ACC_MAG_OFFSET, ACC_MAG_BITS);
//  delay(10000);
  int new_acc_theta = convert_float((*data).acc_theta, ACC_THETA_MIN, ACC_THETA_STEP);
  encode_data (new_acc_theta, ACC_THETA_OFFSET, ACC_THETA_BITS);
  int new_acc_phi = convert_float((*data).acc_phi, ACC_PHI_MIN, ACC_PHI_STEP);
  encode_data (new_acc_phi, ACC_PHI_OFFSET, ACC_PHI_BITS);
  // GPS Data
  int new_altitude = convert_float((*data).altitude, ALT_MIN, ALT_STEP);
  encode_data (new_altitude, ALT_OFFSET, ALT_BITS);
  long new_latitude = convert_double((*data).latitude, LAT_MIN, LAT_STEP);
  int* lat1 = (int*) &new_latitude;
  encode_data (lat1[0], LAT_OFFSET+(LAT_BITS-16), 16);
  encode_data (lat1[1], LAT_OFFSET, LAT_BITS-16);  
  long new_longitude = convert_double((*data).longitude, LONGITUDE_MIN, LONG_STEP);
  int* long1 = (int*) &new_longitude;
  encode_data (long1[0], LONG_OFFSET+(LONG_BITS-16), 16);
  encode_data (long1[1], LONG_OFFSET, LONG_BITS-16);
  // Velovity Data
  int new_vel_magnitude = convert_float((*data).vel_magnitude, VEL_MAG_MIN, VEL_MAG_STEP);
  encode_data (new_vel_magnitude, VEL_MAG_OFFSET, VEL_MAG_BITS);
  int new_vel_theta = convert_float((*data).vel_theta, VEL_THETA_MIN, VEL_THETA_STEP);
  encode_data (new_vel_theta, VEL_THETA_OFFSET, VEL_THETA_BITS);
  int new_vel_phi = convert_float((*data).vel_phi, VEL_PHI_MIN, VEL_PHI_STEP);
  encode_data (new_vel_phi, VEL_PHI_OFFSET, VEL_PHI_BITS);
  // Helium Data
  encode_data ((*data).helium, HEL_OFFSET, HEL_BITS);
  // Ballast Data
  encode_data ((*data).ballast, BALL_OFFSET, BALL_BITS);
  // Battery Data
  int new_battery_percentage = convert_float((*data).BatteryPercentage, BATT_MIN, BATT_STEP);
  encode_data (new_battery_percentage, BATT_OFFSET, BATT_BITS);
  // Time Data
  int time = millis()/1000;
  encode_data (time, TIME_OFFSET, TIME_BITS);
  // State Data
  encode_data ((*data).state, STATE_OFFSET, STATE_BITS);
  // Control Mode Data
  encode_data ((*data).control_mode, CTRL_OFFSET, CTRL_BITS);
  // Emergency Data
  encode_data ((*data).emergency, EMRG_OFFSET, EMRG_BITS);
}

void decode_message (struct Incoming_Data *data) {
  // Initiallize temporary float variable
  float temp;
  // Set recieved values using incoming message bits
  data->altitude = decode_data (REC_ALT_MIN, REC_ALT_STEP, REC_ALT_OFFSET, REC_ALT_LENGTH);
  data->altitude_buffer = decode_data (REC_ALT_BUF_MIN, REC_ALT_BUF_STEP, REC_ALT_BUF_OFFSET, REC_ALT_BUF_LENGTH);
  data->lat_degree = decode_data (REC_LAT_MIN, REC_LAT_STEP, REC_LAT_OFFSET, REC_LAT_LENGTH);

  double lat1 = decode_data (0, 1, REC_LAT_MIN_OFFSET+10, REC_LAT_MIN_LENGTH-10);
  double lat2 = decode_data (0, 1, REC_LAT_MIN_OFFSET, 10);
  double lat_val = lat2*pow(2,REC_LONG_MIN_LENGTH-10) + lat1;
  Serial.println("latitude");
  Serial.println(lat1);
  Serial.println(lat2);
  data->lat_deg_min = (lat_val*REC_LAT_MIN_STEP)+REC_LAT_MIN_MIN;

  data->long_deg = decode_data (REC_LONG_MIN, REC_LONG_STEP, REC_LONG_OFFSET, REC_LONG_LENGTH);

  float long1 = decode_data (0, 1, REC_LONG_MIN_OFFSET+10, REC_LONG_MIN_LENGTH-10);
  float long2 = decode_data (0, 1, REC_LONG_MIN_OFFSET, 10);
  double long_val = long2*pow(2,REC_LONG_MIN_LENGTH-10) + long1;
  data->long_deg_min = (long_val*REC_LONG_MIN_STEP)+REC_LONG_MIN_MIN;
  
  temp = decode_data (REC_CUT_MIN, REC_CUT_STEP, REC_CUT_OFFSET, REC_CUT_LENGTH);
  data->cutdown = temp > 2.5;
  data->update_rate = (int)decode_data (REC_UPD_MIN, REC_UPD_STEP, REC_UPD_OFFSET, REC_UPD_LENGTH);
  data->hel_alpha = decode_data (REC_HEL_A_MIN, REC_HEL_A_STEP, REC_HEL_A_OFFSET, REC_HEL_A_LENGTH);
  data->hel_beta = decode_data (REC_HEL_B_MIN, REC_HEL_B_STEP, REC_HEL_B_OFFSET, REC_HEL_B_LENGTH);
  data->hel_gamma = decode_data (REC_HEL_G_MIN, REC_HEL_G_STEP, REC_HEL_G_OFFSET, REC_HEL_G_LENGTH);
  data->bal_alpha = decode_data (REC_BAL_A_MIN, REC_BAL_A_STEP, REC_BAL_A_OFFSET, REC_BAL_A_LENGTH);
  data->bal_beta = decode_data (REC_BAL_B_MIN, REC_BAL_B_STEP, REC_BAL_B_OFFSET, REC_BAL_B_LENGTH);
  data->bal_gamma = decode_data (REC_BAL_G_MIN, REC_BAL_G_STEP, REC_BAL_G_OFFSET, REC_BAL_G_LENGTH);
  data->length_vent = decode_data (REC_VENT_TIME_MIN, REC_VENT_TIME_STEP, REC_VENT_TIME_OFFSET, REC_VENT_TIME_LENGTH);
  data->vent_inc = decode_data (REC_VENT_INC_MIN, REC_VENT_INC_STEP, REC_VENT_INC_OFFSET, REC_VENT_INC_LENGTH);
  data->ballast_inc = decode_data (REC_BAL_INC_MIN, REC_BAL_INC_STEP, REC_BAL_INC_OFFSET, REC_BAL_INC_LENGTH);
  data->temp_setpoint = decode_data (REC_TEMP_S_MIN, REC_TEMP_S_STEP, REC_TEMP_S_OFFSET, REC_TEMP_S_LENGTH);
  data->temp_P = decode_data (REC_TEMP_P_MIN, REC_TEMP_P_STEP, REC_TEMP_P_OFFSET, REC_TEMP_P_LENGTH);
  data->temp_I = decode_data (REC_TEMP_I_MIN, REC_TEMP_I_STEP, REC_TEMP_I_OFFSET, REC_TEMP_I_LENGTH);
  data->temp_D = decode_data (REC_TEMP_D_MIN, REC_TEMP_D_STEP, REC_TEMP_D_OFFSET, REC_TEMP_D_LENGTH);
  data->control_mode = (int)decode_data (REC_CTRL_MIN, REC_CTRL_STEP, REC_CTRL_OFFSET, REC_CTRL_LENGTH);
  temp = decode_data (REC_MAN_ADJ_MIN, REC_MAN_ADJ_STEP, REC_MAN_ADJ_OFFSET, REC_MAN_ADJ_LENGTH);
  data->manual_adjust = temp > 0.5;
  temp = decode_data (REC_MAN_SEL_MIN, REC_MAN_SEL_STEP, REC_MAN_SEL_OFFSET, REC_MAN_SEL_LENGTH);
  data->manual_select = temp > 0.5;
  data->manual_amount = (int)decode_data (REC_MAN_AMT_MIN, REC_MAN_AMT_STEP, REC_MAN_AMT_OFFSET, REC_MAN_AMT_LENGTH);
  
}

// Convert raw integer to compressed data type
int convert_int (int data, float min, float step) {
  // Convert data to integer equivalent in terms of steps from minimum
  int new_data = int(roundf(float(data - min)/step));
  return new_data;
}

long convert_double (double data, float min, float step) {
  // Convert data to long equivalent in terms of steps from minimum
  long new_data = long(round(double(data - min)/step));
  return new_data;
}

// Convert raw float to compressed data type
int convert_float (float data, float min, float step) {
  // Convert data to integer equivalent in terms of steps from minimum
  int new_data = int(roundf((data - min)/step));
  return new_data;
}

// Encode compressed data into the outgoing_message buffer
void encode_data (int data, int bit_offset, int bit_length) {
  // Encode binary data into appropriate space in buffer
  int first_byte = bit_offset >> 3;
  int last_byte = (bit_offset + bit_length) >> 3;
  int first_bit;
  int last_bit;
  int count = bit_length-1;
  for (int j = first_byte; j <= last_byte; j++) {
    if (j == first_byte) {
      last_bit = 8-bit_offset%8-1; 
    } else {
      last_bit = 7;
    }
    if (j == last_byte) {
      first_bit = 8-(bit_offset+bit_length)%8;
    } else {
      first_bit = 0;
    }
    for (int i = last_bit; i >= first_bit; i--) {
       bitWrite(buffer[j], i, bitRead(data, count));
       count--;
    }
  }
} 

float decode_data (float min, float step, int bit_offset, int bit_length) {
  // Decode binary data from buffer
  int data;
  int first_byte = bit_offset >> 3;
  int last_byte = (bit_offset + bit_length) >> 3;
  int first_bit;
  int last_bit;
  int count = bit_length-1;
  for (int j = first_byte; j <= last_byte; j++) {
    if (j == first_byte) {
      last_bit = 8-bit_offset%8-1; 
    } else {
      last_bit = 7;
    }
    if (j == last_byte) {
      first_bit = 8-(bit_offset+bit_length)%8;
    } else {
      first_bit = 0;
    }
    for (int i = last_bit; i >= first_bit; i--) {
       bitWrite(data, count, bitRead(buffer[j], i));
       count--;
    }
  }
  return (float)(data*step)+min;
}

//bool ISBDCallback(void) // Called while waiting for retries during communications
//{
//   digitalWrite(13, HIGH);       // sets the digital pin 13 on
//   delay(1000);                  // waits for 1 second
//   digitalWrite(13, LOW);        // sets the digital pin 13 off
//   delay(1000);                  // waits for 1 second
//   return true;
//}

//#if DIAGNOSTICS
//void ISBDConsoleCallback(IridiumSBD *device, char c)
//{
//  WiredSerial.write(c);
//}
//
//void ISBDDiagsCallback(IridiumSBD *device, char c)
//{
//  WiredSerial.write(c);
//}
//#endif


