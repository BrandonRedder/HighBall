#include "Control.h"

// min altitude of 60 kft default
// max altitude of 80 kft default
int min_altitude = 60000;
int max_altitude = 80000;

void set_altitude(int lower_bound, int upper_bound) {
  /* Set minimum and maximum altitude bounds for the altitude control system
   *
   * Inputs
   * ------
   *  lower_bound: int representing the lowest acceptable altitude
   *  upper_bound: int representing the highest acceptable altitude
   */
  min_altitude = lower_bound;
  max_altitude = upper_bound;
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
  if (alt<min_altitude && velocity<=0 && accel<=0) {
    /* Below altitude range
     * Moving downward or not moving
     * Accelerating downard or not accelerating
     */
    return DROP_BALLAST;
  } else if (alt>max_altitude && velocity>=0 && accel>=0) {
    /* Above altitude range
     * Moving upward or not moving
     * Accelerating upward or not accelerating
     */
    return RELEASE_HELIUM;
  } // end if
  return NO_ACTION;
} // end altitude_control
