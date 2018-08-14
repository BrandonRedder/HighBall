#include "Sensors.h"
#include "Actuators.h"
#include "Communications.h"
#include "Control.h"
#include "State_Machine.h"

void setup() 
{
  setup_Sensors();
  setup_Actuators();
  setup_Communications();
  setup_Control();
  setup_State_Machine();
}

void loop() 
{
  
}
