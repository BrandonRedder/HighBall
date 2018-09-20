#ifndef CONTROL_H
#define CONTROL_H

struct helium_ballast {
  int helium; // Seconds helium release is open
  int ballast; // Number of rotations of ballast release
};

// TODO: Consider removing min accel and velocity parameters
class altitude_control {
  public:
    altitude_control();
    altitude_control(float, float, float, float);
    void set_altitude_range(float, float);
    void set_rates(float, float);
    helium_ballast get_action(float, float, float);

    // get functions
    float get_min_altitude();
    float get_max_altitude();
    float get_min_velocity();
    float get_min_accel();
    float get_bal_alpha();
    float get_bal_beta();
    float get_bal_gamma();
    float get_hel_alpha();
    float get_hel_beta();
    float get_hel_gamma();
    
    // set functions
    void set_min_altitude(float);
    void set_max_altitude(float);
    void set_min_velocity(float);
    void set_min_accel(float);
    void set_bal_alpha(float);
    void set_bal_beta(float);
    void set_bal_gamma(float);
    void set_hel_alpha(float);
    void set_hel_beta(float);
    void set_hel_gamma(float);

  private:
    float min_altitude;
    float max_altitude;
    float min_velocity;
    float min_accel;
    float bal_alpha;
    float bal_beta;
    float bal_gamma;
    float hel_alpha;
    float hel_beta;
    float hel_gamma;
};

#endif /* ifndef CONTROL_H */
