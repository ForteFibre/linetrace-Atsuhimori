#include "motor.h"

#include <Motor.hpp>

extern Motor motor_0;
extern Motor motor_1;

void motorStop()
{
  motor_0.set_output(0);
  motor_1.set_output(0);
}

void motorRun()
{
  motor_0.set_output(0.3);
  motor_1.set_output(0.3);
}
