#include "motor.h"

#include <mbed.h>

#include <Motor.hpp>

extern Motor motor_0;
extern Motor motor_1;

void motorStop()
{
  motor_0.set_output(0);
  motor_1.set_output(0);
  //ThisThread::sleep_for(100ms);
}

void motorRun()
{
  motor_0.set_output(0.6);
  motor_1.set_output(0.6);
  //ThisThread::sleep_for(50ms);
}
