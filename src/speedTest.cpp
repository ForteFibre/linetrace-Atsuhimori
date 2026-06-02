#include "speedTest.h"

#include <mbed.h>

#include <Encoder.hpp>
#include <cstdio>

#include "motor.h"

extern Encoder encoder_0;
extern Encoder encoder_1;

static Timer timer;
static bool initialized = false;

void speedTestUpdate()
{
  if (!initialized) {
    timer.start();
    initialized = true;
  }

  encoder_0.update();
  encoder_1.update();

  // 右側だけ指令
  setMotor(0.7f, 0.0f);

  if (timer.elapsed_time() >= 100ms) {
    printf("E0:%8lld  E1:%8lld\n", encoder_0.raw_position(), encoder_1.raw_position());

    timer.reset();
  }
}