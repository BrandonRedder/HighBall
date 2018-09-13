#include <SparkFunMPU9250-DMP.h>
#include<math.h>
float calibration[3];

//IMU
MPU9250_DMP imu;
void setup()
{
  SerialUSB.begin(115200); //set baud rate

  // Check communication with IMU
  while(imu.begin() != 0)
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

  //set calibration
  imu.update(UPDATE_ACCEL);
  /*calibration[0] = imu.calcAccel(imu.ax);
  calibration[1] = imu.calcAccel(imu.ay);
  calibration[2] = imu.calcAccel(imu.az);
  float mag = sqrt(calibration[0] * calibration[0] +
    calibration[1] * calibration[1] +
    calibration[2] * calibration[2]);
  calibration[0] = calibration[0]/mag;
  calibration[1] = calibration[1]/mag;
  calibration[2] = calibration[2]/mag;
  */
  calibration[0]=0;
  calibration[1]=0;
  calibration[2]=1;
}//end setup_IMU_01

void loop()
{
  //IMU_OUTPUT updateValues;
  if ( imu.fifoAvailable() > 0 )//if there is new data
  {
    if (imu.dmpUpdateFifo() == 0)//update data
    {
      //update orientation
      imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
      imu.computeEulerAngles(false); //output in radians
      printTest();
            /*
      updateValues.xMag = imu.calcMag(imu.mx);
      updateValues.yMag = imu.calcMag(imu.my);
      updateValues.xMag = imu.calcMag(imu.mz);
      updateValues.q0 = imu.qw;
      updateValues.q1 = imu.qx;
      updateValues.q2 = imu.qy;
      updateValues.q3 = imu.qz;
      */
      }
  }
  return;// updateValues;
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

void printTest(){
  //float theta = imu.roll;
  //float psi = imu.pitch;
  //float phi = imu.yaw;
  float orientation[3];
  double theta;
  double psi;
  double phi;
  calculateAngles(imu.calcQuat(imu.qw), imu.calcQuat(imu.qx), imu.calcQuat(imu.qy), imu.calcQuat(imu.qz), theta, psi, phi);

  float qi = imu.calcQuat(imu.qx);
  float qj = imu.calcQuat(imu.qy);
  float qk = imu.calcQuat(imu.qz);
  float qr = imu.calcQuat(imu.qw);
  //calculate rotation from calibration using pitch - roll - yaw roration matrix
  orientation[0] = calibration[0] * (cos(theta) * cos(phi)) + calibration[1] * (cos(theta) * sin(phi)) - calibration[2] * sin(theta);
  orientation[1] = calibration[0] * (sin(psi) * sin(theta) * cos(phi) - cos(psi) * sin(phi)) + calibration[1] * (sin(psi) * sin(theta) * sin(phi) + cos(psi) * cos(phi)) + calibration[2] * (cos(theta) * sin(psi));
  orientation[2] = calibration[0] * 	(cos(psi) * sin(theta) * cos(phi) + sin(psi) * sin(phi)) + calibration[1] * (cos(psi) * sin(theta) * sin(phi) - sin(psi) * cos(phi)) + calibration[2] * cos(theta) * cos(psi);

  //orientation[0] = calibration[0] * (1 - 2 * (qj * qj + qk * qk)) + calibration[1] * 2 * ( qi * qj - qk * qr) + calibration[2] * 2 * (qi * qk + qj * qr);
  //orientation[1] = calibration[0] * 2 * (qi * qj + qk * qr) + calibration[1] * (1 - 2 * (qi * qi + qk * qk)) + calibration[2] * 2 * (qi * qk - qi * qr);
  //orientation[2] = calibration[0] * 2 * (qi * qk - qj * qr) + calibration[1] * 2 * (qj * qk + qi * qr) + calibration[2] * (1 - 2 * (qi * qi + qj * qj));

  //calculate dot product to project vertical velocity and acceleration onto orientation
  float accelerationVertical = -1*(orientation[0] * -1*imu.calcAccel(imu.ay) + orientation[1] * -1* imu.calcAccel(imu.ax) + orientation[2] * imu.calcAccel(imu.az));
  float velocityVertical = orientation[0] * imu.calcGyro(imu.gx) + orientation[1] * imu.calcGyro(imu.gy) + orientation[2] * imu.calcGyro(imu.gz);
  SerialUSB.println("Q:" + String(imu.calcQuat(imu.qw),4) + ", " + String(imu.calcQuat(imu.qx),4) + ", " + String(imu.calcQuat(imu.qy),4) + ", " + String(imu.calcQuat(imu.qz), 4));
  SerialUSB.println("Mag:" + String(imu.calcMag(imu.mx)) + ", " + String(imu.calcMag(imu.my)) + ", " + String(imu.calcMag(imu.mz)));
  SerialUSB.println("Gyro:" + String(imu.calcGyro(imu.gx)) + ", " + String(imu.calcGyro(imu.gy)) + ", " + String(imu.calcGyro(imu.gz)));
  SerialUSB.println("Accel:" + String(imu.calcAccel(imu.ax)) + ", " + String(imu.calcAccel(imu.ay)) + ", " + String(imu.calcAccel(imu.az)));
  SerialUSB.println("Vertical Acceleration: " + String(accelerationVertical));
  SerialUSB.println("Vertical Velocity: " + String(velocityVertical));
  SerialUSB.println("Orientation  " + String(orientation[0]) + " , " + String(orientation[1]) + " , " + String(orientation[2]));
  SerialUSB.println("Calibration  " + String(calibration[0]) + " , " + String(calibration[1]) + " , " + String(calibration[2]));
  SerialUSB.println("Pitch: " + String(psi) + " Roll: " + String(theta) + " Yaw: " + String(phi));
  SerialUSB.println();
}
