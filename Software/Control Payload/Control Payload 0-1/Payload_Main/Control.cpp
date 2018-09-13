#include "Control.h"

// min and max altitude defaults [ft] 
#define MIN_ALTITUDE 60000
#define MAX_ALTITUDE 80000
// min velocity and acceleration defaults
#define MIN_VELOCITY 0
#define MIN_ACCEL 0

// default constructor, uses default altitudes, velocity, and acceleration
altitude_control::altitude_control() {
  set_altitude_range(MIN_ALTITUDE, MAX_ALTITUDE);
  set_rates(MIN_VELOCITY, MIN_ACCEL);
}

// constructor, allows user specified altitudes, velocity and acceleration
altitude_control::altitude_control(float _min_altitude, float _max_altitude,
                                   float _min_velocity, float _min_accel) {
  set_altitude_range(_min_altitude, _max_altitude);
  set_rates(_min_velocity, _min_accel);
}

void altitude_control::set_altitude_range(float _min_altitude, float _max_altitude) {
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

#define NO_ACTION 0
#define DROP_BALLAST 1
#define RELEASE_HELIUM 2

int altitude_control::get_action(float alt, float velocity, float accel) {
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
   *  int: 0, 1, or 2 corresponding to no action, drop ballast, or release
   *       helium
   */
  if (alt<get_min_altitude()
      && velocity<=get_min_velocity()
      && accel<=get_min_accel()) {
    /* Below altitude range
     * Not moving upward fast enough
     * Not enough upward acceleration
     */
    return DROP_BALLAST;
  } else if (alt>get_max_altitude() 
             && velocity>=(0-get_min_velocity())
             && accel>=(0-get_min_accel())) {
    /* Above altitude range
     * Not moving downward fast enough
     * Not enough downard acceleration
     */
    return RELEASE_HELIUM;
  } // end if
  return NO_ACTION;
} // end altitude_control

// get functions for each member variable
float altitude_control::get_min_altitude() {return(min_altitude);}

float altitude_control::get_max_altitude() {return(max_altitude);}

float altitude_control::get_min_velocity() {return(min_velocity);}

float altitude_control::get_min_accel() {return(min_accel);}

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
