#include "speedControl.h"

#include <mbed.h>

#include <Encoder.hpp>
#include <cstdio>

#include "motor.h"

extern Encoder encoder_0;
extern Encoder encoder_1;

// =========================
// 最終目標速度
// =========================

static float targetLeft = 0.0f;
static float targetRight = 0.0f;

// =========================
// PIが追従する目標速度
// =========================

static float currentTargetLeft = 0.0f;
static float currentTargetRight = 0.0f;

// =========================
// 制御周期
// =========================

static const float DT = 0.005f;

// =========================
// 加速度制限
// 1周期で増減する最大値
// =========================

static const float ACCEL_LIMIT = 0.30f;

// =========================
// フィードフォワード
// =========================

static const float FF_GAIN = 0.035f;

// =========================
// PIゲイン
// =========================

static const float KP_SPEED = 0.02f;
static const float KI_SPEED = 0.010f;

// =========================
// 積分制限
// =========================

static const float INTEGRAL_LIMIT = 20.0f;

// =========================

static float leftIntegral = 0.0f;
static float rightIntegral = 0.0f;

// =========================

static Timer debugTimer;
static bool initialized = false;

// =========================

void setTargetSpeed(float left, float right)
{
  targetLeft = left;
  targetRight = right;
}

// =========================

static float approach(float current, float target)
{
  float diff = target - current;

  if (diff > ACCEL_LIMIT) {
    diff = ACCEL_LIMIT;
  }

  if (diff < -ACCEL_LIMIT) {
    diff = -ACCEL_LIMIT;
  }

  return current + diff;
}

// =========================

void speedControlUpdate()
{
  if (!initialized) {
    debugTimer.start();
    initialized = true;
  }

  encoder_0.update();
  encoder_1.update();

  // 左モータ = encoder_1
  // 右モータ = encoder_0

  float leftSpeed = -encoder_1.speed();
  float rightSpeed = -encoder_0.speed();

  // =========================
  // 加速度制限
  // =========================

  currentTargetLeft = approach(currentTargetLeft, targetLeft);

  currentTargetRight = approach(currentTargetRight, targetRight);

  // =========================
  // 誤差
  // =========================

  float leftError = currentTargetLeft - leftSpeed;

  float rightError = currentTargetRight - rightSpeed;

  // =========================
  // 積分
  // =========================

  leftIntegral += leftError * DT;
  rightIntegral += rightError * DT;

  if (leftIntegral > INTEGRAL_LIMIT) {
    leftIntegral = INTEGRAL_LIMIT;
  }

  if (leftIntegral < -INTEGRAL_LIMIT) {
    leftIntegral = -INTEGRAL_LIMIT;
  }

  if (rightIntegral > INTEGRAL_LIMIT) {
    rightIntegral = INTEGRAL_LIMIT;
  }

  if (rightIntegral < -INTEGRAL_LIMIT) {
    rightIntegral = -INTEGRAL_LIMIT;
  }

  // =========================
  // FF
  // =========================

  float leftFF = currentTargetLeft * FF_GAIN;

  float rightFF = currentTargetRight * FF_GAIN;

  // =========================
  // PI
  // =========================

  float leftPwm = leftFF + KP_SPEED * leftError + KI_SPEED * leftIntegral;

  float rightPwm = rightFF + KP_SPEED * rightError + KI_SPEED * rightIntegral;

  // =========================
  // PWM制限
  // =========================

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

  // =========================
  // debug
  // =========================

  if (debugTimer.elapsed_time() >= 100ms) {
    printf(
      "TL:%5.1f "
      "TR:%5.1f "
      "LS:%5.1f "
      "RS:%5.1f "
      "LP:%4.2f "
      "RP:%4.2f\n",

      targetLeft, targetRight, leftSpeed, rightSpeed, leftPwm, rightPwm);

    debugTimer.reset();
  }
}

// =========================

void resetSpeedControl()
{
  targetLeft = 0.0f;
  targetRight = 0.0f;

  currentTargetLeft = 0.0f;
  currentTargetRight = 0.0f;

  leftIntegral = 0.0f;
  rightIntegral = 0.0f;

  setMotor(0.0f, 0.0f);
}