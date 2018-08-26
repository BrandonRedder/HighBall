# High Ball  

## Team Makeup
### Group Members  
* Mathew Manning
* Brandon Redder
* David Richardson
* David Sanchez
* Kristy Scott
* Kyle Watters

### Project Sponsor and Advisors  
**Sponsor:** MIT Lincoln Laboratories  
**Point of Contact:** Navid Yazdani  

**Primary Advisor:** Dr. Morris Cohen  
**Additional Advisor:** Dr. Whit Smith

## Project Goal
The goal of this project is to develop an altitude and location control system 
for a latex HAB to allow long duration flights over a general location. 
Algorithms take into account current pressure and temperature conditions when 
venting helium or dropping ballast to estimate altitude changes. This will
allow for inexpensive controlled balloon flights for a variety of payloads.

## Libraries
* Pressure Sensor (https://github.com/sparkfun/SparkFun_MS5803-14BA_Breakout_Arduino_Library)
* GPS (https://github.com/sparkfun/SparkFun_I2C_GPS_Arduino_Library)
* TinyGPS++ (http://arduiniana.org/libraries/tinygpsplus/)
* IMU (https://github.com/sparkfun/SparkFun_MPU-9250-DMP_Arduino_Library)

## TODO
- [ ] Sensor Code
  - [ ] Pressure Sensor
  - [ ] Temperature Sensor
  - [ ] IMU
  - [ ] GPS
  - [ ] Battery Monitoring
  - [ ] Tests
- [ ] Sensor Hardware Connections
  - [ ] Assembly
  - [ ] Tests
- [ ] Communication Code
  - [ ] Setup
  - [ ] Send Functionality
  - [ ] Receive Functionality
  - [ ] Tests
- [ ] Communication Hardware Connections
  - [ ] Assembly
  - [ ] Tests
- [ ] Actuators Code
  - [ ] Setup
  - [ ] Ballast Release
  - [ ] Hellium Release
  - [ ] Cutdown
  - [ ] Tests
- [ ] Actuator Hardware Connections
  - [ ] Assembly
  - [ ] Tests
- [ ] Final Payload Assembly
  - [ ] Electronic Assembly
  - [ ] Mechanical Assembly
  - [ ] Tests
- [ ] Control Algorithm
  - [ ] Algorithm Development
  - [ ] Control Loop Implementation
  - [ ] Feedback Implementation
  - [ ] Altitude Control
  - [ ] Position Control
  - [ ] Tests
- [ ] User Interface
  - [ ] Design
  - [ ] Connection With Payload
  - [ ] Interface implementation
  - [ ] Tests
- [ ] Main Loop
  - [ ] System Initialization
  - [ ] Determine Update Frequency
  - [ ] Update Location and Direction
  - [ ] Adjust Altitude
  - [ ] Monitor Wind Conditions
  - [ ] Adjust Position
  - [ ] Minimize Power Use
  - [ ] Tests
