#include <SparkFunMPU9250-DMP.h>

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
  SerialUSB.println("Q:" + String(imu.calcQuat(imu.qw),4) + ", " + String(imu.calcQuat(imu.qx),4) + ", " + String(imu.calcQuat(imu.qy),4) + ", " + String(imu.calcQuat(imu.qz), 4));
  SerialUSB.println("Mag:" + String(imu.calcMag(imu.mx)) + ", " + String(imu.calcMag(imu.my)) + ", " + String(imu.calcMag(imu.mz)));
  SerialUSB.println("Gyro:" + String(imu.calcGyro(imu.gx)) + ", " + String(imu.calcGyro(imu.gy)) + ", " + String(imu.calcGyro(imu.gz)));
  SerialUSB.println("Accel:" + String(imu.calcAccel(imu.ax)) + ", " + String(imu.calcAccel(imu.ay)) + ", " + String(imu.calcAccel(imu.az)));
  SerialUSB.println();
}
