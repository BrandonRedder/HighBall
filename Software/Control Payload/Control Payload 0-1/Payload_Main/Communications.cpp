#include "Communications.h"

IridiumSBD modem(IridiumSerial, SLEEP_PIN, RING_PIN);

void setup_Communications(void)
{
  // Set LED to output for display
  pinMode(13, OUTPUT);

  // Start the serial ports
  WiredSerial.begin(115200);
  while (!Serial);
  IridiumSerial.begin(19200);

  // Setup the Iridium modem
  modem.setPowerProfile(IridiumSBD::DEFAULT_POWER_PROFILE); // high current setting for battery power
  int start = modem.begin(); // start the modem
  int signalQuality = -1;
  int err = modem.getSignalQuality(signalQuality); // get signal quality
  if (start != ISBD_SUCCESS) {
	  if (status == PRE_LAUNCH_STATE) {
      WiredSerial.print("Start Iridium failed: error ");
      WiredSerial.println(start);
    }
  }
  if (err != ISBD_SUCCESS) {
	  if (status == PRE_LAUNCH_STATE) {
      WiredSerial.print("Get Signal quality failed: error ");
      WiredSerial.println(err);
	  }
  } else {
	  if (status == PRE_LAUNCH_STATE) {
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

// Call Iridium module from state machine 
void call_iridium(void) {
  // Check if the modulel is asleep/ responds
  int err = modem.sleep();
  if (err != ISBD_SUCCESS && err != ISBD_IS_ASLEEP) {
	if (status == PRE_LAUNCH_STATE) {
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
    send_message();
  }
  delay(5000);
}

void send_message(void) {
  size_t bufferSize = sizeof(buffer);
	
  int err;
  err = modem.sendReceiveSBDBinary(buffer, 30, buffer, bufferSize);
 
  if (err != ISBD_SUCCESS)
  {
    messageSent = false;
    if (status == PRE_LAUNCH_STATE) {
      WiredSerial.print("Message failed to send");
      WiredSerial.println(err);
    }
  }
  else // success!
  {
    messageSent = true;
    if (status == PRE_LAUNCH_STATE) {
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
    decode_message();
  }
}

void check_status (void) {
  switch (status) {
    case PRE_LAUNCH_STATE: { Message_Rate = 5*60;
                       break;
                     }
    case FLIGHT_STATE: { Message_Rate = 5*60;
                   break;
                 }
    case FALLING_STATE: { Message_Rate = 20;
                    break;
                  }
    case RECOVERY_STATE: { Message_Rate = 5*60;
                     break;
                   }
    case TEST_STATE: { Message_Rate = 5*60;
                 break;
               }
    default: { Message_Rate = 5*60;
               break;
             }
  }
}

void encode_all (void) {
  // Temperature Data
  float temperature0 =  temperature_sensor::read_temp(); 
  int new_temperature = convert_float(temperature0, TEMP_MIN, TEMP_STEP);
  encode_data (new_temperature, TEMP_OFFSET, TEMP_BITS);
  // Pressure Data
  float pressure = pressure_sensor::read_pressure();
  int new_pressure = convert_float(pressure, PRS_MIN, PRS_STEP);
  encode_data (new_pressure, PRS_OFFSET, PRS_BITS);
  // IMU Data
  convert_IMU_data();
  int new_acc_magnitude = convert_float(acc_magnitude, ACC_MAG_MIN, ACC_MAG_STEP);
  encode_data (new_acc_magnitude, ACC_MAG_OFFSET, ACC_MAG_BITS);
  int new_acc_theta = convert_float(acc_theta, ACC_THETA_MIN, ACC_THETA_STEP);
  encode_data (new_acc_theta, ACC_THETA_OFFSET, ACC_THETA_BITS);
  int new_acc_phi = convert_float(acc_phi, ACC_PHI_MIN, ACC_PHI_STEP);
  encode_data (new_acc_phi, ACC_PHI_OFFSET, ACC_PHI_BITS);
  // GPS Data
  float altitude0 = GPS::get_alt(); 
  int new_altitude = convert_float(altitude0, ALT_MIN, ALT_STEP);
  encode_data (new_altitude, ALT_OFFSET, ALT_BITS);
  float latitude0 = GPS::get_lat(); 
  int new_latitude = convert_float(latitude0, LAT_MIN, LAT_STEP);
  encode_data (new_latitude, LAT_OFFSET, LAT_BITS);
  float longitude0 = GPS::get_long(); 
  int new_longitude = convert_float(longitude0, LONGITUDE_MIN, LONG_STEP);
  encode_data (new_longitude, LONG_OFFSET, LONG_BITS);
  // Battery Data
  float BatteryPercentage = check_battery();
  int new_battery_percentage = convert_float(BatteryPercentage, BATT_MIN, BATT_STEP);
  encode_data (new_battery_percentage, BATT_OFFSET, BATT_BITS);
  // Velovity Data
    // Add here
  // Ballast Data
    // Add here
}

void decode_message (void) {
  // First byte of message tells command type
  int command_byte = (int)(buffer[0]);
  
  if (command_byte == 10) {
    // Command 1
  }
    
  if (command_byte == 20) {
    // Command 2
  }
    
  if (command_byte == 30) {
    // Command 3
  }
    
  if (command_byte == 40) {
    // Command 4
  }
    
  if (command_byte == 50) {
    // Command 5
  }
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

// Encode compressed data into the outgoing_message buffer
void encode_data (int data, int bit_offset, int bit_length) {
  // Encode binary data into appropriate space in buffer
  int first_byte = bit_offset >> 3;
  int last_byte = (bit_offset + bit_length) >> 3;
  int first_bit;
  int last_bit;
  int count = 0;
  for (int j = first_byte; j <= last_byte; j++) {
    if (j == first_byte) {
      first_bit = 0;
    } else {
      first_byte = bit_offset%8; 
    }
    if (j == last_byte) {
      last_bit = (bit_offset+bit_length)%8;
    } else {
      last_bit = 8;
    }
    for (int i = first_bit; i < last_bit; i++) {
       bitWrite(buffer[j], i, bitRead(data, count));
       count++;
    }
  }
} 

float decode_data (float min, float step, int bit_offset, int bit_length) {
  // Encode binary data into appropriate space in buffer
  int data;
  int first_byte = bit_offset >> 3;
  int last_byte = (bit_offset + bit_length) >> 3;
  int first_bit;
  int last_bit;
  int count = 0;
  for (int j = first_byte; j <= last_byte; j++) {
    if (j == first_byte) {
      first_bit = 0;
    } else {
      first_byte = bit_offset%8; 
    }
    if (j == last_byte) {
      last_bit = (bit_offset+bit_length)%8;
    } else {
      last_bit = 8;
    }
    for (int i = first_bit; i < last_bit; i++) {
       bitWrite(data, count, bitRead(buffer[j], i));
       count++;
    }
  }
  return (float)(data*step)+min;
}

void convert_IMU_data(void) {
  IMU_OUTPUT imu_data = read_IMU_01();
  acc_magnitude = sqrtf(powf(imu_data.q0,2) + powf(imu_data.q1,2) + powf(imu_data.q3,2));
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
