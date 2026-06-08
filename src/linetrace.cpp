#include "lineTrace.h"

#include <mbed.h>

#include <cstdio>

#include "motor.h"
#include "sensor.h"
#include "setting.h"
#include "speedControl.h"
#include "speedMode.h"

extern DigitalOut led_c;

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
// ロスト探索
// =========================

static int lostCounter = 0;

// =========================
// デバッグ
// =========================

static Timer debugTimer;
static Timer lostLedTimer;
static bool lostLedActive = false;

static Timer crossTimer;
static bool crossRunning = false;

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

    lostLedTimer.start();

    initialized = true;
  }

  float error = getLinePosition();

#if ENABLE_CROSS_STRAIGHT

  // =========================
  // 交差点直進
  // =========================

  if (isCrossLine()) {
    crossRunning = true;

    crossTimer.reset();
  }

  if (crossRunning) {
    if (crossTimer.elapsed_time() < std::chrono::milliseconds(CROSS_STRAIGHT_TIME_MS)) {
      if (driveMode == SPEED_CONTROL) {
        setTargetSpeed(baseSpeed, baseSpeed);

      } else {
        setMotor(basePwm, basePwm);
      }

      return;
    }

    crossRunning = false;
  }

#endif

  // =========================
  // ラインロスト探索
  // =========================

#if ENABLE_LINE_LOST_SEARCH

  if (isLineLost()) {
    led_c = 1;

    lostLedActive = true;

    lostLedTimer.reset();

    integral = 0.0f;

    int dir = getLostDirection();

    // =========================
    // SPEED CONTROL
    // =========================

    if (driveMode == SPEED_CONTROL) {
      if (dir > 0) {
        setTargetSpeed(LOST_SEARCH_SPEED_STAGE2, -LOST_SEARCH_SPEED_STAGE2);

      } else {
        setTargetSpeed(-LOST_SEARCH_SPEED_STAGE2, LOST_SEARCH_SPEED_STAGE2);
      }
    }

    // =========================
    // DIRECT PWM
    // =========================

    else {
      if (dir > 0) {
        setMotor(LOST_SEARCH_PWM_STAGE2, -LOST_SEARCH_PWM_STAGE2);

      } else {
        setMotor(-LOST_SEARCH_PWM_STAGE2, LOST_SEARCH_PWM_STAGE2);
      }
    }

#if DEBUG_LINE_TRACE

    if (debugTimer.elapsed_time() >= 100ms) {
      printf("[LOST] Dir:%d\n", dir);

      debugTimer.reset();
    }

#endif

    return;
  }

#endif

  // =========================
  // ライン発見
  // =========================

  if (lostLedActive) {
    if (lostLedTimer.elapsed_time() >= 3000ms) {
      led_c = 0;

      lostLedActive = false;
    }
  }

  lostCounter = 0;

  integral += error * DT;

  //==========================
  // 積分制限
  //==========================

  if (integral > LINE_INTEGRAL_LIMIT) {
    integral = LINE_INTEGRAL_LIMIT;
  }

  if (integral < -LINE_INTEGRAL_LIMIT) {
    integral = -LINE_INTEGRAL_LIMIT;
  }

  float derivative = (error - previousError) / DT;

  previousError = error;

  // ==========================================
  // SPEED CONTROL MODE
  // ==========================================

  if (driveMode == SPEED_CONTROL) {
    float turn = -(speedKp * error + speedKi * integral + speedKd * derivative);

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
    float turn = (pwmKp * error + pwmKi * integral + pwmKd * derivative);

    float leftPwm = basePwm + turn;

    float rightPwm = basePwm - turn;

    if (leftPwm > 1.0f) leftPwm = 1.0f;

    if (leftPwm < -1.0f) leftPwm = -1.0f;

    if (rightPwm > 1.0f) rightPwm = 1.0f;

    if (rightPwm < -1.0f) rightPwm = -1.0f;

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

  lostCounter = 0;

  resetSpeedControl();
}
