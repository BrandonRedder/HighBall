#ifndef CONTROL_H
#define CONTROL_H

//Constants

//Functions
//Main Setup Function
void set_altitude(int lower_bound, int upper_bound);
int altitude_control(float alt, float velocity, float accel);
// void setup_Control(void);

#endif /* ifndef CONTROL_H */
