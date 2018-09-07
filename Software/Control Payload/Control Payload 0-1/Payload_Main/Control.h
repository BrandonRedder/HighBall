#ifndef CONTROL_H
#define CONTROL_H

class altitude_control {
  public:
    altitude_control();
    altitude_control(float, float, float, float);
    void set_altitude_range(float, float);
    void set_rates(float, float);
    int get_action(float, float, float);

    float get_min_altitude();
    float get_max_altitude();
    float get_min_velocity();
    float get_min_accel();
    void set_min_altitude(float);
    void set_max_altitude(float);
    void set_min_velocity(float);
    void set_min_accel(float);

  private:
    float min_altitude;
    float max_altitude;
    float min_velocity;
    float min_accel;
};

#endif /* ifndef CONTROL_H */
