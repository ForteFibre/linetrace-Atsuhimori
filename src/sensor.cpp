#include "sensor.h"

#include <mbed.h>

#include <AnalogBus.hpp>

extern AnalogBus sensor;

static int raw[6];

static float normalized[6];

static int sensor_max[6];
static int sensor_min[6];

static Timer calibrationTimer;

static int weight[6] = {2500, 1500, 500, -500, -1500, -2500};

void sensorUpdate()
{
  for (int i = 0; i < 6; i++) {
    raw[i] = sensor.read_u16(i);

    int min = sensor_min[i];
    int max = sensor_max[i];

    if (max == min) {
      normalized[i] = 0.0f;
    } else {
      normalized[i] = (float)(raw[i] - min) / (max - min);
    }

    if (normalized[i] < 0.0f) {
      normalized[i] = 0.0f;
    }

    if (normalized[i] > 1.0f) {
      normalized[i] = 1.0f;
    }
  }
}

int readRaw(int index) { return raw[index]; }

float readNormalized(int index) { return normalized[index]; }

float getLinePosition()
{
  float numerator = 0.0f;

  float denominator = 0.0f;

  for (int i = 0; i < 6; i++) {
    numerator += normalized[i] * weight[i];

    denominator += normalized[i];
  }

  // ライン見失い
  if (denominator == 0.0f) {
    return 0.0f;
  }

  return numerator / denominator;
}

void calibrationStart()
{
  for (int i = 0; i < 6; i++) {
    sensor_max[i] = 0;
    sensor_min[i] = 65535;
  }

  calibrationTimer.reset();
  calibrationTimer.start();
}

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

bool calibrationFinished()
{
  if (calibrationTimer.elapsed_time() >= 7000ms) {
    calibrationTimer.stop();

    return true;
  }

  return false;
}

/*
float readNormalized(int index)
{
  int raw = sensor.read_u16(index);

  int min = sensor_min[index];
  int max = sensor_max[index];

  if (max == min) {
    return 0.0f;
  }

  float normalized = (float)(raw - min) / (max - min);

  if (normalized < 0.0f) {
    normalized = 0.0f;
  }

  if (normalized > 1.0f) {
    normalized = 1.0f;
  }
  return normalized;
}
  */
