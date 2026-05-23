#include "motor.h"
//足回り
#include <mbed.h>

#include <Motor.hpp>

extern Motor motor_0;
extern Motor motor_1;

static const float LEFT_SIGN = -1.0f;

static const float RIGHT_SIGN = -1.0f;

void setMotor(float left, float right)
{
  motor_0.set_output(left * LEFT_SIGN);
  motor_1.set_output(right * RIGHT_SIGN);
}

void motorStop()
{
  motor_0.set_output(0.0f);
  motor_1.set_output(0.0f);
  //ThisThread::sleep_for(100ms);
}

void motorRun()
{
  motor_0.set_output(0.4f);  //左
  motor_1.set_output(0.4f);  //右
  //ThisThread::sleep_for(50ms);
}
