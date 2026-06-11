#include "sensor.h"

#include <mbed.h>

#include <AnalogBus.hpp>
#include <cstdio>

#include "setting.h"

extern AnalogBus sensor;

// =====================================
// センサー保存
// =====================================

static int raw[6];

static float normalized[6];

// =====================================
// キャリブレーション
// =====================================

static int sensor_max[6];

static int sensor_min[6];

static Timer calibrationTimer;

// =====================================
// デバッグ
// =====================================

static Timer debugTimer;

// =====================================
// デバッグ状態保持
// =====================================

static bool prevCrossLine = false;

static bool prevLineLost = false;

// =====================================
// 重み
// 右 → 左
// =====================================

static int weight[6] = {1000, 600, 200, -200, -600, -1000};

// =====================================
// ライン状態
// =====================================

static float lastPosition = 0.0f;

static int lastSensor = 2;

static bool lineLost = false;

static bool crossLine = false;

// =====================================
// ロスト方向
//
// 1 = 右
// -1 = 左
// =====================================

static int lostDirection = 1;

// =====================================
// カウンタ
// =====================================

static int lostCounter = 0;

static int crossCounter = 0;

static int recoverCounter = 0;
// =====================================
// sensorUpdate
// =====================================

static float rightHistory = 0.0f;
static float leftHistory = 0.0f;

static int historyCounter = 0;

static int lastEdgeDirection = 1;

// 1 = 右端(sensor0)
// -1 = 左端(sensor5)

static const float dirWeight[6] = {30.0f, 4.0f, 1.0f, 1.0f, 4.0f, 30.0f};

void sensorUpdate()
{
  static bool initialized = false;

  if (!initialized) {
    debugTimer.start();
    initialized = true;
  }

  for (int i = 0; i < 6; i++) {
    raw[i] = sensor.read_u16(i);

    int min = sensor_min[i];
    int max = sensor_max[i];

    if (max == min) {
      normalized[i] = 0.0f;
    } else {
      normalized[i] = (float)(raw[i] - min) / (float)(max - min);
    }

    if (normalized[i] < 0.0f) {
      normalized[i] = 0.0f;
    }

    if (normalized[i] > 1.0f) {
      normalized[i] = 1.0f;
    }
  }
}

// =====================================
// getLinePosition
// =====================================

float getLinePosition()
{
  float numerator = 0.0f;

  float denominator = 0.0f;

  float maxLineValue = 0.0f;

  int maxIndex = lastSensor;

  int whiteCount = 0;

  float rightSide = 0.0f;
  float leftSide = 0.0f;

  // =================================
  // 白線値生成
  // =================================

  for (int i = 0; i < 6; i++) {
    float lineValue = 1.0f - normalized[i];

    numerator += lineValue * weight[i];

    denominator += lineValue;

    if (lineValue > maxLineValue) {
      maxLineValue = lineValue;
      maxIndex = i;
    }

    // 端センサ記憶

    if (i == 0 && lineValue > WHITE_DETECT_THRESHOLD) {
      lastEdgeDirection = 1;
    }

    if (i == 5 && lineValue > WHITE_DETECT_THRESHOLD) {
      lastEdgeDirection = -1;
    }

    if (lineValue > WHITE_DETECT_THRESHOLD) {
      whiteCount++;
    }
    if (i <= 2) {
      rightSide += lineValue * dirWeight[i];

    } else {
      leftSide += lineValue * dirWeight[i];
    }
  }

#if LOST_DIRECTION_MODE

  if (!lineLost) {
    rightHistory += rightSide;

    leftHistory += leftSide;

    historyCounter++;

    if (historyCounter >= LOST_DIRECTION_HISTORY) {
      rightHistory = 0.0f;
      leftHistory = 0.0f;

      historyCounter = 0;
    }
  }

#endif

  // =================================
  // 交差点判定
  // =================================

  if (whiteCount >= CROSS_WHITE_COUNT) {
    crossCounter++;
  } else {
    crossCounter = 0;
  }

#if ENABLE_CROSS_STRAIGHT

  crossLine = (crossCounter >= CROSS_COUNT_THRESHOLD);

#else

  crossLine = false;

#endif
  // =================================
  // ロスト判定
  // =================================

  if (denominator < LOST_THRESHOLD) {
    lostCounter++;

    recoverCounter = 0;
  } else {
    recoverCounter++;

    if (recoverCounter >= LOST_RECOVER_COUNT) {
      lostCounter = 0;
    }
  }

#if ENABLE_LINE_LOST_SEARCH

  lineLost = (lostCounter >= LOST_COUNT_THRESHOLD);

#else

  lineLost = false;

#endif

  // =================================
  // 最後に見た方向保存
  // =================================

#if LOST_DIRECTION_MODE == 0

  if (!lineLost) {
    if (rightSide > leftSide) {
      lostDirection = 1;
    } else {
      lostDirection = -1;
    }
  }

#elif LOST_DIRECTION_MODE == 1

  if (!lineLost) {
    if (rightHistory > leftHistory) {
      lostDirection = 1;
    } else {
      lostDirection = -1;
    }
  }

#elif LOST_DIRECTION_MODE == 2

  if (!lineLost) {
    lostDirection = lastEdgeDirection;
  }

#endif

#if DEBUG_SENSOR

  if (debugTimer.elapsed_time() >= 1000ms) {
    printf(
      "[SNS] "
      "%.2f %.2f %.2f %.2f %.2f %.2f "
      "Den:%.2f "
      "White:%d "
      "Cross:%d "
      "Lost:%d "
      "Dir:%d "
      "Last:%d\n",
      normalized[0], normalized[1], normalized[2], normalized[3], normalized[4], normalized[5],
      denominator, whiteCount, crossLine, lineLost, lostDirection, lastSensor);

    printf("R:%.1f L:%.1f RH:%.1f LH:%.1f\n", rightSide, leftSide, rightHistory, leftHistory);

#if DEBUG_CROSS

    if (crossLine && !prevCrossLine) {
      printf("[CROSS START]\n");
    }

    if (!crossLine && prevCrossLine) {
      printf("[CROSS END]\n");
    }

    prevCrossLine = crossLine;

#endif

#if DEBUG_LOST

    if (lineLost && !prevLineLost) {
      printf("[LOST START] Dir:%d\n", lostDirection);
    }

    if (!lineLost && prevLineLost) {
      printf("[LOST END]\n");
    }

    prevLineLost = lineLost;

#endif

    debugTimer.reset();
  }

#endif

  // =================================
  // ロスト中
  // =================================

  if (lineLost) {
    return lastPosition;
  }

  // =================================
  // 通常計算
  // =================================

  lastSensor = maxIndex;

  float position = numerator / denominator;

  lastPosition = position;

  return position;
}

// =====================================

bool isLineLost() { return lineLost; }

// =====================================

bool isCrossLine() { return crossLine; }

// =====================================

int getLastSensor() { return lastSensor; }

// =====================================

int getLostDirection() { return lostDirection; }

// =====================================

int readRaw(int index) { return raw[index]; }

// =====================================

float readNormalized(int index) { return normalized[index]; }

// =====================================

void calibrationStart()
{
  for (int i = 0; i < 6; i++) {
    sensor_max[i] = 0;
    sensor_min[i] = 65535;
  }

  calibrationTimer.reset();
  calibrationTimer.start();
}

// =====================================

void calibrationUpdate()
{
  for (int i = 0; i < 6; i++) {
    int value = sensor.read_u16(i);

    if (value > sensor_max[i]) {
      sensor_max[i] = value;
    }

    if (value < sensor_min[i]) {
      sensor_min[i] = value;
    }
  }
}

// =====================================

bool calibrationFinished()
{
  if (calibrationTimer.elapsed_time() >= 7000ms) {
    calibrationTimer.stop();
    return true;
  }

  return false;
}