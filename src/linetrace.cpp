#include "lineTrace.h"

#include <mbed.h>

#include <cstdio>

#include "motor.h"
#include "sensor.h"
#include "setting.h"
#include "speedControl.h"
#include "speedMode.h"

// =========================
// 制御周期
// =========================

static const float DT = 0.005f;

// =========================
// SPEED_CONTROL用
// =========================

static float baseSpeed = 20.0f;

static float speedKp = 0.002f;
static float speedKi = 0.0f;
static float speedKd = 0.0f;

// =========================
// DIRECT_PWM用
// =========================

static float basePwm = 0.55f;

static float pwmKp = 0.0002f;
static float pwmKi = 0.0f;
static float pwmKd = 0.0f;

// =========================
// PID内部変数
// =========================

static float integral = 0.0f;
static float previousError = 0.0f;

// =========================
// デバッグ
// =========================

static Timer debugTimer;

// =========================

void setLineTracePID(float p, float i, float d)
{
  speedKp = p;
  speedKi = i;
  speedKd = d;
}

// =========================

void setDirectPwmPID(float p, float i, float d)
{
  pwmKp = p;
  pwmKi = i;
  pwmKd = d;
}

// =========================

void setBaseSpeed(float speed) { baseSpeed = speed; }

// =========================

void setBasePwm(float pwm) { basePwm = pwm; }

// =========================

void lineTraceUpdate()
{
  static bool initialized = false;

  if (!initialized) {
    debugTimer.start();
    initialized = true;
  }

  float error = getLinePosition();

  integral += error * DT;

  float derivative = (error - previousError) / DT;

  previousError = error;

  // ==========================================
  // SPEED CONTROL MODE
  // ==========================================

  if (driveMode == SPEED_CONTROL) {
    float turn = speedKp * error + speedKi * integral + speedKd * derivative;

    float leftTarget = baseSpeed + turn;

    float rightTarget = baseSpeed - turn;

    setTargetSpeed(leftTarget, rightTarget);

#if DEBUG_LINE_TRACE

    if (debugTimer.elapsed_time() >= 100ms) {
      printf(
        "[SPD] "
        "Pos:%7.1f "
        "LT:%6.1f "
        "RT:%6.1f\n",

        error, leftTarget, rightTarget);

      debugTimer.reset();
    }

#endif
  }

  // ==========================================
  // DIRECT PWM MODE
  // ==========================================

  else {
    float turn = pwmKp * error + pwmKi * integral + pwmKd * derivative;

    float leftPwm = basePwm + turn;

    float rightPwm = basePwm - turn;

    if (leftPwm > 1.0f) {
      leftPwm = 1.0f;
    }

    if (leftPwm < -1.0f) {
      leftPwm = -1.0f;
    }

    if (rightPwm > 1.0f) {
      rightPwm = 1.0f;
    }

    if (rightPwm < -1.0f) {
      rightPwm = -1.0f;
    }

    setMotor(leftPwm, rightPwm);

#if DEBUG_LINE_TRACE

    if (debugTimer.elapsed_time() >= 100ms) {
      printf(
        "[PWM] "
        "Pos:%7.1f "
        "LP:%5.2f "
        "RP:%5.2f\n",

        error, leftPwm, rightPwm);

      debugTimer.reset();
    }

#endif
  }
}

// =========================

void resetLineTrace()
{
  integral = 0.0f;

  previousError = 0.0f;

  resetSpeedControl();
}