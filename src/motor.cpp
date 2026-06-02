#include "motor.h"

#include <mbed.h>

#include <Motor.hpp>

extern Motor motor_0;
extern Motor motor_1;

static const float LEFT_SIGN = -1.0f;
static const float RIGHT_SIGN = -1.0f;

void setMotor(float left, float right)
{
  // motor_1 が左
  // motor_0 が右

  motor_1.set_output(left * LEFT_SIGN);
  motor_0.set_output(right * RIGHT_SIGN);
}

void motorStop()
{
  motor_0.set_output(0.0f);
  motor_1.set_output(0.0f);
}

void motorRun() { setMotor(0.4f, 0.4f); }