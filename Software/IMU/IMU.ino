#include <SparkFunMPU9250-DMP.h>
#include<math.h>
float zCalibration[3];
float xCalibration[3];
float yCalibration[3];
//IMU
MPU9250_DMP imu;

float zAcceleration;
float horizontalAcceleration;
float direction;

boolean output_single_on = false;
boolean output_comms_test = false;

void setup()
{
  SerialUSB.begin(115200); //set baud rate
  Serial.begin(9600);
  // Check communication with IMU
  while(imu.begin() != INV_SUCCESS)
  {
      SerialUSB.println("Unable to connect with IMU.");
      delay(5000);
  }//end while display
  
  //enable 6-axis quaternion outpu
  imu.dmpBegin(DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL, 50);//check sampling rate

  //enable XYZ magnetometer for absolute orientation determination
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
  imu.setGyroFSR(2000);
  imu.setAccelFSR(2);
  imu.setSampleRate(50);
  imu.setCompassSampleRate(50);//check sampling rate
  imu.update(UPDATE_ACCEL);

  //set calibration
  zCalibration[0]=0; //x
  zCalibration[1]=0; //y
  zCalibration[2]=1; //z

  xCalibration[0] = 1; //x
  xCalibration[1] = 0; //y
  xCalibration[2] = 0; //z

  yCalibration[0] = 0; //x
  yCalibration[1] = 1; //y
  yCalibration[2] = 0; //z
  
}//end setup_IMU_01

void loop()
{
  // Can add comms test code as an additional command here
  if (Serial.available() >= 2) {
    if (Serial.read() == '#') { // Start of control message
      int command = Serial.read(); // read command
      if (command == 'f') { // Request Data
        output_single_on = true;
      } else if (command == 'c') { // Reply to Comms test
        output_comms_test = true;
      }
    }
  }
  /*if(Serial.available() > 0){
    comm = Serial.read();
    if(comm == '1'){
      Serial.print('2');//may change back to '1'
    }
    else if (comm == '0') {
      Serial.print('0');
    } else if (comm == '2') {
      exportValues(zAcceleration, horizontalAcceleration, direction);
    }
  } */
  //IMU_OUTPUT updateValues;
  if ( imu.fifoAvailable() )//if there is new data
  {
    if (imu.dmpUpdateFifo() == 0)//update data
    {
      //update orientation
      SerialUSB.println("inif");
      imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
      imu.computeEulerAngles(false); //output in radians
      calculateValues();
      }
  }

  if (output_single_on) exportValues(zAcceleration, horizontalAcceleration, direction); // send if data is requested
  if (output_comms_test) commsReply();

  output_single_on = false;
  output_comms_test = false;
  return;
  
}

static void calculateAngles(float qw, float qx, float qy, float qz, double& roll, double& pitch, double& yaw){
  //double qy2 = qy * qy;
  //calculate roll
  float t0 = +2.0 * (qw * qx + qy * qz);
  float t1 = +1.0 - 2.0 * ( qx * qx + qy*qy);
  roll = atan2(t0,t1);

  //calculate pitch
  float t2 = +2.0 * (qw * qy  - qz * qx);
  if(fabs(t2) >= 1){
    pitch = copysign(M_PI/2, t2);
  }
  else{
    pitch = asin(t2);
  }

  SerialUSB.println("T2 = " + String(t2));
  SerialUSB.println("qw  " + String(qw) +  "   qx  " + String(qx) +  "   qy  " + String(qy) +  "   qz  " + String(qz));
  float t3 = +2.0 * (qw * qz + qx * qy);
  float t4 = +1.0 - 2.0 * (qy*qy + qz * qz);
  yaw = atan2(t3, t4);

}

void calculateValues(){

  float zOrientation[3];
  float xOrientation[3];
  float yOrientation[3];

  double theta; //roll
  double psi; //pitch
  double phi; //yaw

  calculateAngles(imu.calcQuat(imu.qw), imu.calcQuat(imu.qx), imu.calcQuat(imu.qy), imu.calcQuat(imu.qz), theta, psi, phi);

  float qi = imu.calcQuat(imu.qx);
  float qj = imu.calcQuat(imu.qy);
  float qk = imu.calcQuat(imu.qz);
  float qr = imu.calcQuat(imu.qw);

  //calculate rotation from calibration using pitch - roll - yaw roration matrix
  zOrientation[0] = zCalibration[0] * (cos(theta) * cos(phi)) + zCalibration[1] * (cos(theta) * sin(phi)) - zCalibration[2] * sin(theta);
  zOrientation[1] = zCalibration[0] * (sin(psi) * sin(theta) * cos(phi) - cos(psi) * sin(phi)) + zCalibration[1] * (sin(psi) * sin(theta) * sin(phi) + cos(psi) * cos(phi)) + zCalibration[2] * (cos(theta) * sin(psi));
  zOrientation[2] = zCalibration[0] *	(cos(psi) * sin(theta) * cos(phi) + sin(psi) * sin(phi)) + zCalibration[1] * (cos(psi) * sin(theta) * sin(phi) - sin(psi) * cos(phi)) + zCalibration[2] * cos(theta) * cos(psi);

  xOrientation[0] = xCalibration[0] * (cos(theta) * cos(phi)) + xCalibration[1] * (cos(theta) * sin(phi)) - xCalibration[2] * sin(theta);
  xOrientation[1] = xCalibration[0] * (sin(psi) * sin(theta) * cos(phi) - cos(psi) * sin(phi)) + xCalibration[1] * (sin(psi) * sin(theta) * sin(phi) + cos(psi) * cos(phi)) + xCalibration[2] * (cos(theta) * sin(psi));
  xOrientation[2] = xCalibration[0] *	(cos(psi) * sin(theta) * cos(phi) + sin(psi) * sin(phi)) + xCalibration[1] * (cos(psi) * sin(theta) * sin(phi) - sin(psi) * cos(phi)) + xCalibration[2] * cos(theta) * cos(psi);

  yOrientation[0] = yCalibration[0] * (cos(theta) * cos(phi)) + yCalibration[1] * (cos(theta) * sin(phi)) - yCalibration[2] * sin(theta);
  yOrientation[1] = yCalibration[0] * (sin(psi) * sin(theta) * cos(phi) - cos(psi) * sin(phi)) + yCalibration[1] * (sin(psi) * sin(theta) * sin(phi) + cos(psi) * cos(phi)) + yCalibration[2] * (cos(theta) * sin(psi));
  yOrientation[2] = yCalibration[0] *	(cos(psi) * sin(theta) * cos(phi) + sin(psi) * sin(phi)) + yCalibration[1] * (cos(psi) * sin(theta) * sin(phi) - sin(psi) * cos(phi)) + yCalibration[2] * cos(theta) * cos(psi);

  //calculate dot product to project acceleration onto orientation
  zAcceleration = -1*(zOrientation[0] * -1*imu.calcAccel(imu.ay) + zOrientation[1] * -1* imu.calcAccel(imu.ax) + zOrientation[2] * imu.calcAccel(imu.az));
  float xAcceleration = -1*(xOrientation[0] * -1*imu.calcAccel(imu.ay) + xOrientation[1] * -1* imu.calcAccel(imu.ax) + xOrientation[2] * imu.calcAccel(imu.az));
  float yAcceleration = -1*(yOrientation[0] * -1*imu.calcAccel(imu.ay) + yOrientation[1] * -1* imu.calcAccel(imu.ax) + yOrientation[2] * imu.calcAccel(imu.az));

  direction;

  if(xAcceleration < 0){
    direction = atan(yAcceleration/xAcceleration)*180/M_PI + 180;
  }
  else if((xAcceleration > 0) && (yAcceleration >= 0)){
    direction = atan(yAcceleration/xAcceleration)*180/M_PI;
  }
  else if((xAcceleration > 0) && (yAcceleration <= 0)){
    direction = atan(yAcceleration/xAcceleration)*180/M_PI + 360;
  }
  else if(xAcceleration == 0){
    direction = yAcceleration > 0 ? M_PI/2 : 3*M_PI/2;
  }

  horizontalAcceleration = sqrt(xAcceleration * xAcceleration + yAcceleration * yAcceleration);
  //exportValues(zAcceleration, horizontalAcceleration, direction);

  SerialUSB.println("Q:" + String(imu.calcQuat(imu.qw),4) + ", " + String(imu.calcQuat(imu.qx),4) + ", " + String(imu.calcQuat(imu.qy),4) + ", " + String(imu.calcQuat(imu.qz), 4));
  SerialUSB.println("Accel:" + String(-1*imu.calcAccel(imu.ay)) + ", " + String(-1*imu.calcAccel(imu.ax)) + ", " + String(imu.calcAccel(imu.az)));
  SerialUSB.println("Vertical Acceleration: " + String(zAcceleration));
  SerialUSB.println("OrientationZ  " + String(zOrientation[0]) + " , " + String(zOrientation[1]) + " , " + String(zOrientation[2]));
  SerialUSB.println("OrientationX  " + String(xOrientation[0]) + " , " + String(xOrientation[1]) + " , " + String(xOrientation[2]));
  SerialUSB.println("OrientationY  " + String(yOrientation[0]) + " , " + String(yOrientation[1]) + " , " + String(yOrientation[2]));
  SerialUSB.println("Horizontal Acceleration: " + String(horizontalAcceleration) + " Angle: " + String(direction,4));
  SerialUSB.println("Y acceleration: " + String(yAcceleration) + " X Acceleration: " + String(xAcceleration));
  SerialUSB.println("Pitch: " + String(psi) + " Roll: " + String(theta) + " Yaw: " + String(phi));
  SerialUSB.println();
}

void exportValues(float Vaccel, float Haccel, float dir){
  //Serial.print("<");
  Serial.print("H ");
  Serial.print(Vaccel); Serial.print(",");
  Serial.print(Haccel); Serial.print(",");
  Serial.print(dir); Serial.println();
}

void commsReply() {
  Serial.print("C ");
  Serial.print(2.0);
}
/*
void printTest(){
  SerialUSB.println("Q:" + String(imu.calcQuat(imu.qw),4) + ", " + String(imu.calcQuat(imu.qx),4) + ", " + String(imu.calcQuat(imu.qy),4) + ", " + String(imu.calcQuat(imu.qz), 4));
  SerialUSB.println("Accel:" + String(-1*imu.calcAccel(imu.ay)) + ", " + String(-1*imu.calcAccel(imu.ax)) + ", " + String(imu.calcAccel(imu.az)));
  SerialUSB.println("Vertical Acceleration: " + String(zAcceleration));
  SerialUSB.println("OrientationZ  " + String(zOrientation[0]) + " , " + String(zOrientation[1]) + " , " + String(zOrientation[2]));
  SerialUSB.println("OrientationX  " + String(xOrientation[0]) + " , " + String(xOrientation[1]) + " , " + String(xOrientation[2]));
  SerialUSB.println("OrientationY  " + String(yOrientation[0]) + " , " + String(yOrientation[1]) + " , " + String(yOrientation[2]));
  SerialUSB.println("Horizontal Acceleration: " + String(horizontalAcceleration) + " Angle: " + String(direction,4));
  SerialUSB.println("Y acceleration: " + String(yAcceleration) + " X Acceleration: " + String(xAcceleration));
  SerialUSB.println("Pitch: " + String(psi) + " Roll: " + String(theta) + " Yaw: " + String(phi));
  SerialUSB.println();
}*/
