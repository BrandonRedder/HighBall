#include "Control.h"

// min and max altitude defaults [ft] 
#define MIN_ALTITUDE 60000
#define MAX_ALTITUDE 80000
// min velocity and acceleration defaults
#define MIN_VELOCITY 0
#define MIN_ACCEL 0
// default alpha, beta, gamma weights
#define DEFAULT_BAL_ALPHA 1
#define DEFAULT_BAL_BETA 1
#define DEFAULT_BAL_GAMMA 1
#define DEFAULT_HEL_ALPHA 1
#define DEFAULT_HEL_BETA 1
#define DEFAULT_HEL_GAMMA 1

// default constructor, uses default altitudes, velocity, and acceleration
altitude_control::altitude_control() {
  set_altitude_range(MIN_ALTITUDE, MAX_ALTITUDE);
  set_rates(MIN_VELOCITY, MIN_ACCEL);
  set_bal_alpha(DEFAULT_BAL_ALPHA);
  set_bal_beta(DEFAULT_BAL_BETA);
  set_bal_gamma(DEFAULT_BAL_GAMMA);
  set_hel_alpha(DEFAULT_HEL_ALPHA);
  set_hel_beta(DEFAULT_HEL_BETA);
  set_hel_gamma(DEFAULT_HEL_GAMMA);
}

// constructor, allows user specified altitudes, velocity and acceleration
altitude_control::altitude_control(float _min_altitude, float _max_altitude,
                                   float _min_velocity, float _min_accel) {
  set_altitude_range(_min_altitude, _max_altitude);
  set_rates(_min_velocity, _min_accel);
  set_bal_alpha(DEFAULT_BAL_ALPHA);
  set_bal_beta(DEFAULT_BAL_BETA);
  set_bal_gamma(DEFAULT_BAL_GAMMA);
  set_hel_alpha(DEFAULT_HEL_ALPHA);
  set_hel_beta(DEFAULT_HEL_BETA);
  set_hel_gamma(DEFAULT_HEL_GAMMA);
}

void altitude_control::set_altitude_range(float _min_altitude, 
                                          float _max_altitude) {
  /* Set minimum and maximum altitude bounds for the altitude control system
   *
   * Inputs
   * ------
   *  _min_altitude: float representing the lowest acceptable altitude
   *  _max_altitude: float representing the highest acceptable altitude
   */
  set_min_altitude(_min_altitude);
  set_max_altitude(_max_altitude);
}

void altitude_control::set_rates(float _min_velocity, float _min_accel) {
  /* Set minimum velocity and acceleration values for the balloon to be
   * considered moving or accelerating.
   *
   * Inputs
   * ------
   *  _min_velocity: float representing the lowest velocity to be considered
   *                moving
   *  _min_accel: float representing the lowest acceleration to be considered
   *             accelerating
   */
  set_min_velocity(_min_velocity);
  set_min_accel(_min_accel);
}

helium_ballast altitude_control::get_action(float alt,
                                            float velocity,
                                            float accel) {
  /* Determines what action to take to maintain altitude between min_alittude
   * and max_altitude
   *
   * Inputs
   * ------
   *  alt: float representing the balloon's current altitude in feet
   *  velocity: float representing the balloon's current vertical velocity;
   *            positive is upward, negative is downward
   *  accel: float representing the balloon's current vertical acceleration;
   *         positive is upward, negative is downward
   *
   * Returns
   * -------
   *  helium_ballast: struct containing helium and ballast release amounts
   */
  helium_ballast response;
  float delta_altitude = 0;
  float ballast_score;
  float helium_score;

  // calculate delta altitude
  // negative = below min_altitude
  // positive = above max_altitude
  if (alt < get_min_altitude()) {
    delta_altitude = alt - get_min_altitude();
  } else if (alt > get_max_altitude()) {
    delta_altitude = alt - get_max_altitude();
  }

  // Apply weighting to delta_altitude, velocity, and acceleration
  ballast_score = get_bal_alpha() * delta_altitude
                  + get_bal_beta() * velocity
                  + get_bal_gamma() * accel;

  helium_score = get_hel_alpha() * delta_altitude
                 + get_hel_beta() * velocity
                 + get_hel_gamma() * accel;

  // if balast score > 0 do not release any ballast
  if (ballast_score > 0) {
    response.ballast = 0;
  } else {
    response.ballast = int (0-ballast_score);
  }

  if (helium_score < 0) {
    response.helium = 0;
  } else {
    response.helium = int (helium_score);
  }

  return(response);
} // end altitude_control

// get functions for each member variable
float altitude_control::get_min_altitude() {return(min_altitude);}

float altitude_control::get_max_altitude() {return(max_altitude);}

float altitude_control::get_min_velocity() {return(min_velocity);}

float altitude_control::get_min_accel() {return(min_accel);}

// delta altitude ballast weighting
float altitude_control::get_bal_alpha() {return(bal_alpha);} 
// velocity ballast weighting
float altitude_control::get_bal_beta() {return(bal_beta);} 
// acceleration ballast weighting
float altitude_control::get_bal_gamma() {return(bal_gamma);} 

// delta altitude helium weighting
float altitude_control::get_hel_alpha() {return(hel_alpha);} 
// velocity helium weighting
float altitude_control::get_hel_beta() {return(hel_beta);} 
// acceleration helium weighting
float altitude_control::get_hel_gamma() {return(hel_gamma);} 

// set functions for each member variable
void altitude_control::set_min_altitude(float _min_altitude) {
  min_altitude = _min_altitude;
}

void altitude_control::set_max_altitude(float _max_altitude) {
  max_altitude = _max_altitude;
}

void altitude_control::set_min_velocity(float _min_velocity) {
  min_velocity = _min_velocity;
}

void altitude_control::set_min_accel(float _min_accel) {
  min_accel = _min_accel;
}

void altitude_control::set_bal_alpha(float _alpha) {
  bal_alpha = _alpha;
}

void altitude_control::set_bal_beta(float _beta) {
  bal_beta = _beta;
}

void altitude_control::set_bal_gamma(float _gamma) {
  bal_gamma = _gamma;
}

void altitude_control::set_hel_alpha(float _alpha) {
  hel_alpha = _alpha;
}

void altitude_control::set_hel_beta(float _beta) {
  hel_beta = _beta;
}

void altitude_control::set_hel_gamma(float _gamma) {
  hel_gamma = _gamma;
}
