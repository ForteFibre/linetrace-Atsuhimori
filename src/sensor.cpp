#include "sensor.h"

#include <mbed.h>

#include <AnalogBus.hpp>

extern AnalogBus sensor;

int sensor_max[6];
int sensor_min[6];

static Timer calibrationTimer;

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
  if (calibrationTimer.elapsed_time() >= 7s) {
    calibrationTimer.stop();

    return true;
  }

  return false;
}
