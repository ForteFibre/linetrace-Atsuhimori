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

static int weight[6] = {2500, 1500, 500, -500, -1500, -2500};

// =========================
// 最後ライン位置
// =========================

static float lastPosition = 0.0f;

// =========================
// ラインロスト回数
// =========================

static int lostCount = 0;

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

  for (int i = 0; i < 6; i++) {
    numerator += normalized[i] * weight[i];

    denominator += normalized[i];
  }

  // =========================
  // ラインロスト判定
  // =========================

  if (denominator < LOST_THRESHOLD) {
    lostCount++;

    if (lostCount > 20) {
      lostCount = 20;
    }

    float searchPosition = 2500.0f + (lostCount * 100.0f);

    if (searchPosition > 4000.0f) {
      searchPosition = 4000.0f;
    }

    if (lastPosition > 0) {
      return searchPosition;
    } else {
      return -searchPosition;
    }
  }

  // =========================
  // ライン発見
  // =========================

  lostCount = 0;

  // =========================
  // 通常位置計算
  // =========================

  float position = numerator / denominator;

  lastPosition = position;

  return position;
}

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