#include "lineTrace.h"

#include <mbed.h>

#include <cstdio>

#include "sensor.h"
#include "speedControl.h"

// =========================
// 制御周期
// =========================

static const float DT = 0.005f;

// =========================
// 基本速度
// 単位：encoder count / update
// =========================

static float baseSpeed = 20.0f;

// =========================
// PIDゲイン
// =========================

static float kp = 0.002f;

static float ki = 0.0f;

static float kd = 0.0f;

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
  kp = p;
  ki = i;
  kd = d;
}

// =========================

void setBaseSpeed(float speed) { baseSpeed = speed; }

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

  float turn = kp * error + ki * integral + kd * derivative;

  previousError = error;

  // =========================
  // 速度目標生成
  // =========================

  float leftTarget = baseSpeed + turn;

  float rightTarget = baseSpeed - turn;

  setTargetSpeed(leftTarget, rightTarget);

  if (debugTimer.elapsed_time() >= 100ms) {
    printf(
      "Pos:%7.1f "
      "LT:%6.1f "
      "RT:%6.1f\n",

      error, leftTarget, rightTarget);

    debugTimer.reset();
  }
}

// =========================

void resetLineTrace()
{
  integral = 0.0f;

  previousError = 0.0f;

  resetSpeedControl();
}