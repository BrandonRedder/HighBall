include "Communications.h"

IridiumSBD modem(IridiumSerial, SLEEP_PIN, RING,PIN);

void setup_Communications(void)
{
  // Set LED to output for display
  pinMode(13, OUTPUT);

  // Start the serial ports
  WiredSerial.begin(115200);
  while (!Serial);
  IridiumSerial.begin(19200);

  // Setup the Iridium modem
  int signalQuality = -1;
  int power = modem.setPowerProfile(0); // high current setting for battery power
  int begin = modem.begin(); // start the modem
  int err = modem.getSignalQuality(signalQuality); // get signal quality
  if (power != ISBD_SUCCESS)
  {
	if (status == PRE_LAUNCH) {
      WiredSerial.print("Set Power Level failed: error ");
      WiredSerial.println(power);
    }
  }
  if (begin != ISBD_SUCCESS)
  {
	if (status == PRE_LAUNCH) {
      WiredSerial.print("Start Iridium failed: error ");
      WiredSerial.println(begin);
    }
  }
  if (err != ISBD_SUCCESS)
  {
	if (status == PRE_LAUNCH) {
      WiredSerial.print("Get Signal quality failed: error ");
      WiredSerial.println(err);
    }
  }else{
	if (status == PRE_LAUNCH) {
      WiredSerial.print("Signal Quality: ");
      WiredSerial.println(signalQuality);
    }
  }  
}

// New data fields for communications
// Acceleration
float acc_magnitude;
float acc_theta;
float acc_phi;
// Velocity
float vel_magnitude;
float vel_theta;
float vel_phi;

// Set timer between messages and create message sent bit
uint32_t timer = millis();
bool messageSent = true;
int Message_Rate = 5*60;

// 3 Latched alert bits
bool alt_alert = false;
bool temp_alert = false;
bool fall_alert = false;
bool emergency = false;

// 30 Byte incoming and outgoing message buffer
uint8_t buffer[30];
bitset<240> incoming_message;
bitset<240> outgoing_message;

// Call Iridium module from state machine 
void call_iridium(void) {
  // Check if the modulel is asleep/ responds
  err = modem.sleep();
  if (err != ISBD_SUCCESS && err != ISBD_IS_ASLEEP) {
	if (status == PRE_LAUNCH) {
    WiredSerial.print("Sleep failed: error ");
    WiredSerial.println(err);
    }
  }
  
  check_status();
  uint32_t t = millis();
  uint32_t dt = timer - t;
  if (emergency || dt > Message_Rate*1000 || !messageSent) {
    if (messageSent) {
      timer = millis();
      if (emergency) {
	      emergency = false;
      }
    }
    encode_all();
    bitset2buffer();
    send_message();
  }
  delay(5000)
}

void send_message(void)
{
  size_t bufferSize = sizeof(buffer);
	
  int err;
  err = modem.sendReceiveSBDBinary(buffer, 30, buffer, bufferSize);
 
  if (err != ISBD_SUCCESS)
  {
    messageSent = false;
    if (status == PRE_LAUNCH) {
      WiredSerial.print("Message failed to send");
      WiredSerial.plingln(err);
    }
  }
  else // success!
  {
    messageSent = true;
    if (status == PRE_LAUNCH) {
      WiredSerial.print("Inbound buffer size is ");
      WiredSerial.println(bufferSize);
      for (int i=0; i<bufferSize; ++i)
      {
        WiredSerial.print(buffer[i], HEX);
        if (isprint(buffer[i]))
        {
          WiredSerial.print("(");
          WiredSerial.write(buffer[i]);
          WiredSerial.print(")");
        }
        WiredSerial.print(" ");
      }
    }
    buffer2bitset();
    decode_message(bufferSize);
  }
}

void check_status (void) {
  switch (status) {
    case PRE_LAUNCH: {
      Message_Rate = 5*60;
    break;
    }
    case FLIGHT: {
      Message_Rate = 5*60;
      
    break;
    }
    case FALLING: {
      Message_Rate = 20;
    break;
    }
    case RECOVERY: {
      Message_Rate = 5*60;
    break;
    }
    case TEST: {
      Message_Rate = 5*60;
    break;
    }
    default: {
    ...
  }
}

void encode_all (void) {
  // Temperature Data
  int new_temperature = convert_float(temperature, TEMP_MIN, TEMP_STEP);
  encode_data (new_temperature, TEMP_OFFSET, TEMP_BITS);
  // Pressure Data
  int new_pressure = convert_float(pressure, PRS_MIN, PRS_STEP);
  encode_data (new_altitude, PRS_OFFSET, PRS_BITS);
  // IMU Data
  convert_IMU_data();
  int new_acc_magnitude = convert_float(acc_magnitude, ACC_MAG_MIN, ACC_MAG_STEP);
  encode_data (new_acc_magnitude, ACC_MAG_OFFSET, ACC_MAG_BITS);
  int new_acc_theta = convert_float(acc_theta, ACC_THETA_MIN, ACC_THETA_STEP);
  encode_data (new_acc_theta, ACC_THETA_OFFSET, ACC_THETA_BITS);
  int new_acc_phi = convert_float(acc_phi, ACC_PHI_MIN, ACC_PHI_STEP);
  encode_data (new_acc_phi, ACC_PHI_OFFSET, ACC_PHI_BITS);
  // GPS Data
  int new_altitude = convert_float(GSP_Altitude, ALT_MIN, ALT_STEP);
  encode_data (new_altitude, ALT_OFFSET, ALT_BITS);
  int new_latitude = convert_double(GPS_Lat, LAT_MIN, LAT_STEP);
  encode_data (new_latitude, LAT_OFFSSET, LAT_BITS);
  int new_longitude = convert_double(GPS_Long, LONG_MIN, LONG_STEP);
  encode_data (new_longitude, LAONG_OFFSET, LONG_BITS);
  // Battery Data
  int new_battery_percentage = convert_float(Battery_Percentage, BATT_MIN, BATT_STEP);
  encode_data (new_battery_percentage, BATT_OFFSET, BATT_BITS);
  // Velovity Data
    // Add here
  // Ballast Data
    // Add here
}

void decode_message (void) {
  // Temperature Data
  temperature_in = decode_data (TEMP_MIN, TEMP_STEP, TEMP_OFFSET, TEMP_BITS);
  // Pressure Data
  pressure_in = decode_data (PRS_MIN, PRS_STEP, PRS_OFFSET, PRS_BITS);
  // IMU Data
  acc_magnitud_in = decode_data (ACC_MAG_MIN, ACC_MAG_STEP, ACC_MAG_OFFSET, ACC_MAG_BITS);
  acc_theta_in = decode_data (ACC_THETA_MIN, ACC_THETA_STEP, ACC_THETA_OFFSET, ACC_THETA_BITS);
  acc_phi_in = decode_data (ACC_PHI_MIN, ACC_PHI_STEP, ACC_PHI_OFFSET, ACC_PHI_BITS);
  // GPS Data
  GSP_Altitude_in = decode_data (ALT_MIN, ALT_STEP, ALT_OFFSET, ALT_BITS);
  GPS_Lat_in = decode_data (LAT_MIN, LAT_STEP, LAT_OFFSSET, LAT_BITS);
  GPS_Long_in = decode_data (LONG_MIN, LONG_STEP, LAONG_OFFSET, LONG_BITS);
  // Battery Data
  Battery_Percentage = decode_data (BATT_MIN, BATT_STEP, BATT_OFFSET, BATT_BITS);
  // Velovity Data
    // Add here
  // Ballast Data
    // Add here
}

// Convert raw integer to compressed data type
int convert_int (int data, float min, float step) {
  // Convert data to integer equivalent in terms of steps from minimum
  int new_data = int(roundf(float(data - min)/step));
  return new_data;
}

// Convert raw float to compressed data type
int convert_float (float data, float min, float step) {
  // Convert data to integer equivalent in terms of steps from minimum
  int new_data = int(roundf((data - min)/step));
  return new_data;
}

// Convert raw double to compressed data type
int convert_double (double data, float min, float step) {
  // Convert data to integer equivalent in terms of steps from minimum
  int new_data = int(round((data - min)/step));
  return new_data;
}

// Encode compressed data into the outgoing_message buffer
void encode_data (int data, int bit_offset, int bit_length) {
  // Convert integer to binary data with bitset
  bitset<bit_length> bin_data(data);
  // Encode binary data into appropriate space in bitset
  for (int i = 0; i < bit_length; i++) {
    outgoing_message[bit_offset+i] = bin_data[i]; 
    }
  }
}

float decode_data (float min, float step, int offset, int bits) {
  bitset<bits> hold_data;
  for (int i = 0; i < bits; i++) {
    hold_data[i] = incoming_message[offset+i];
  }
  float data = (float)(hold_data.to_ulong());
  return (data*step)+min;
}
  
void bitset2buffer (void) {
  bitset<8> temp_byte;
  for (int i = 0; i < 30; i++) {
    for (int j = 0; j < 8; j++) {
	  temp_byte[j] = outgoing_message[8*i + j];
    }
    buffer[i] = (uint8_t)(temp_byte.to_ulong());
  }
}

void buffer2bitset (void) {
  bitset<8> temp_byte;
  for (int i = 0; i < 30; i++) {
    temp_byte((int)(buffer[i]));
    for (int j = 0; j < 8; j++) {
	  outgoing_message[8*i + j] = temp_byte[j];
    }
  }
}   

void convert_IMU_data(void) {
  IMU_OUTPUT imu_data = read_IMU_01();
  acc_magnitude = sqrtf(powf(imu_data.q0) + powf(imu_data.q1) + powf(imu_data.q3));
  acc_theta = acos(imu_data.q2/acc_magnitude);
  acc_phi = atan2(imu_data.q1,imu_data.q0);
}

bool ISBDCallback(void) // Called while waiting for retries during communications
{
   digitalWrite(13, HIGH);       // sets the digital pin 13 on
   delay(1000);                  // waits for 1 second
   digitalWrite(13, LOW);        // sets the digital pin 13 off
   delay(1000);                  // waits for 1 second
   return true;
}

#if DIAGNOSTICS
void ISBDConsoleCallback(IridiumSBD *device, char c)
{
  WiredSerial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c)
{
  WiredSerial.write(c);
}
#endif