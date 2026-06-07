#include "sensor.h"

//センサーまとめ

#include <mbed.h>

#include <AnalogBus.hpp>

#include "setting.h"

extern AnalogBus sensor;

// =========================
// センサー保存
// =========================

static int raw[6];

static float normalized[6];

// =========================
// キャリブレーション値
// =========================

static int sensor_max[6];

static int sensor_min[6];

// =========================
// キャリブレーションタイマ
// =========================

static Timer calibrationTimer;

// =========================
// ライン位置重み
// 右 → 0 1 2 3 4 5 → 左
// =========================

static int weight[6] = {1000, 600, 200, -200, -600, -1000};

// =========================
// 最後ライン位置
// =========================

static float lastPosition = 0.0f;

// =========================
// 最後に検知したセンサ
// =========================

static int lastSensor = 2;

// =========================
// ラインロスト
// =========================

static bool lineLost = false;

// =========================
// ロスト連続回数
// =========================

static int lostCounter = 0;

// =========================
// センサー更新
// =========================

void sensorUpdate()
{
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

// =========================
// 生値取得
// =========================

int readRaw(int index) { return raw[index]; }

// =========================
// 正規化値取得
// =========================

float readNormalized(int index) { return normalized[index]; }

// =========================
// ライン位置計算
// =========================

float getLinePosition()
{
  float numerator = 0.0f;

  float denominator = 0.0f;

  float maxValue = 0.0f;

  int maxIndex = lastSensor;

  for (int i = 0; i < 6; i++) {
    numerator += normalized[i] * weight[i];

    denominator += normalized[i];

    if (normalized[i] > maxValue) {
      maxValue = normalized[i];

      maxIndex = i;
    }
  }

  // =========================
  // ロスト判定
  // =========================

  if (denominator < LOST_THRESHOLD) {
    lostCounter++;

  } else {
    lostCounter = 0;
  }

  lineLost = (lostCounter >= LOST_COUNT_THRESHOLD);

  if (lineLost) {
    return lastPosition;
  }

  // =========================
  // ライン発見
  // =========================

  lastSensor = maxIndex;

  float position = numerator / denominator;

  lastPosition = position;

  return position;
}

// =========================

bool isLineLost() { return lineLost; }

// =========================

int getLastSensor() { return lastSensor; }

// =========================
// キャリブレーション開始
// =========================

void calibrationStart()
{
  for (int i = 0; i < 6; i++) {
    sensor_max[i] = 0;

    sensor_min[i] = 65535;
  }

  calibrationTimer.reset();

  calibrationTimer.start();
}

// =========================
// キャリブレーション更新
// =========================

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

// =========================
// キャリブレーション終了
// =========================

bool calibrationFinished()
{
  if (calibrationTimer.elapsed_time() >= 7000ms) {
    calibrationTimer.stop();

    return true;
  }

  return false;
}