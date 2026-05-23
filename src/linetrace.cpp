#include "lineTrace.h"
//ライントレース
#include "motor.h"
#include "sensor.h"

static const float BASE_SPEED = 0.35f;

static const float KP = 0.00015f;

void lineTraceUpdate()
{
  float error = getLinePosition();

  float turn = KP * error;

  float left = BASE_SPEED + turn;

  float right = BASE_SPEED - turn;

  if (left > 1.0f) {
    left = 1.0f;
  }

  if (left < -1.0f) {
    left = -1.0f;
  }

  if (right > 1.0f) {
    right = 1.0f;
  }

  if (right < -1.0f) {
    right = -1.0f;
  }

  setMotor(left, right);
}

void resetLineTrace() { setMotor(0.0f, 0.0f); }
