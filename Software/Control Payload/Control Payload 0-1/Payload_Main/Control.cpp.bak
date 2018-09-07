#include "Control.h"

// min altitude of 60 kft default
// max altitude of 80 kft default
float min_altitude = 60000;
float max_altitude = 80000;

void set_altitude(float lower_bound, float upper_bound) {
  /* Set minimum and maximum altitude bounds for the altitude control system
   *
   * Inputs
   * ------
   *  lower_bound: float representing the lowest acceptable altitude
   *  upper_bound: float representing the highest acceptable altitude
   */
  min_altitude = lower_bound;
  max_altitude = upper_bound;
}

// velocity and acceleration cutoff values
// allows adjusting the rate of altitude changes
float min_velocity = 0;
float min_accel = 0;

void set_rates(float velocity_cutoff, float accel_cutoff) {
  /* Set minimum velocity and acceleration values for the balloon to be
   * considered moving or accelerating.
   *
   * Inputs
   * ------
   *  velocity_cut: float representing the lowest velocity to be considered
   *                moving
   *  accel_cut: float representing the lowest acceleration to be considered
   *             accelerating
   */
  min_velocity = velocity_cutoff;
  min_accel = accel_cutoff;
}

#define NO_ACTION 0
#define DROP_BALLAST 1
#define RELEASE_HELIUM 2

int altitude_control(float alt, float velocity, float accel) {
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
  // TODO: acount for the case when acceleration is close to 0, but will not
  // currently trigger ballast or helium release
  if (alt<min_altitude && velocity<=min_velocity && accel<=min_accel) {
    /* Below altitude range
     * Not moving upward fast enough
     * Not enough upward acceleration
     */
    return DROP_BALLAST;
  } else if (alt>max_altitude && velocity>=(0-min_velocity)
             && accel>=(0-min_accel)) {
    /* Above altitude range
     * Not moving downward fast enough
     * Not enough downard acceleration
     */
    return RELEASE_HELIUM;
  } // end if
  return NO_ACTION;
} // end altitude_control
