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

  //enable 6-axis quaternion output
  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL, 50);//check sampling rate

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

void printTest(){
  float theta = imu.roll;
  float psi = imu.pitch;
  float phi = imu.yaw;
  float orientation[3];
  //calculate rotation from calibration using pitch - roll - yaw roration matrix
  orientation[0] = calibration[0] * (cos(theta) * cos(phi)) + calibration[1] * (cos(theta) * sin(phi)) - calibration[2] * sin(theta);
  orientation[1] = calibration [0] * (sin(psi) * sin(theta) * cos(phi) - cos(psi) * sin(phi)) + calibration[1] * (sin(psi) * sin(theta) * sin(phi) + cos(psi) * cos(phi)) + calibration[2] * (cos(theta) * sin(psi));
  orientation[2] = calibration[0] * 	(cos(psi) * sin(theta) * cos(phi) + sin(psi) * sin(phi)) + calibration[1] * (cos(psi) * sin(theta) * sin(phi) - sin(psi) * cos(phi)) + calibration[2] * cos(theta) * cos(psi);

  //calculate dot product to project vertical velocity and acceleration onto orientation
  float accelerationVertical = orientation[0] * imu.calcAccel(imu.ax) + orientation[1] * imu.calcAccel(imu.ay) + orientation[2] * imu.calcAccel(imu.az);
  float velocityVertical = orientation[0] * imu.calcGyro(imu.gx) + orientation[1] * imu.calcGyro(imu.gy) + orientation[2] * imu.calcGyro(imu.gz);
  SerialUSB.println("Q:" + String(imu.calcQuat(imu.qw),4) + ", " + String(imu.calcQuat(imu.qx),4) + ", " + String(imu.calcQuat(imu.qy),4) + ", " + String(imu.calcQuat(imu.qz), 4));
  SerialUSB.println("Mag:" + String(imu.calcMag(imu.mx)) + ", " + String(imu.calcMag(imu.my)) + ", " + String(imu.calcMag(imu.mz)));
  SerialUSB.println("Gyro:" + String(imu.calcGyro(imu.gx)) + ", " + String(imu.calcGyro(imu.gy)) + ", " + String(imu.calcGyro(imu.gz)));
  SerialUSB.println("Accel:" + String(imu.calcAccel(imu.ax)) + ", " + String(imu.calcAccel(imu.ay)) + ", " + String(imu.calcAccel(imu.az)));
  SerialUSB.println("Vertical Acceleration: " + String(accelerationVertical));
  SerialUSB.println("Vertical Velocity: " + String(velocityVertical));
  SerialUSB.println(String(orientation[0]) + " , " + String(orientation[1]) + " , " + String(orientation[2]));
  SerialUSB.println(String(calibration[0]) + " , " + String(calibration[1]) + " , " + String(calibration[2]));
  SerialUSB.println("Pitch: " + String(psi) + " Roll: " + String(theta) + " Yaw: " + String(phi));
  SerialUSB.println();
}
