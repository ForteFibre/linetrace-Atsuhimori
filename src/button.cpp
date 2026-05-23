#include "button.h"
//初期待機
#include <mbed.h>

extern DigitalIn sw1;
extern DigitalIn sw2;

static bool prevSw1 = false;
static bool prevSw2 = false;

static bool sw1Pressed = false;
static bool sw2Pressed = false;

static Timer debounceTimer;

void buttonUpdate()
{
  static bool initialized = false;

  if (!initialized) {
    debounceTimer.start();

    initialized = true;
  }

  bool currentSw1 = !sw1;
  bool currentSw2 = !sw2;

  sw1Pressed = false;
  sw2Pressed = false;

  if (debounceTimer.elapsed_time() > 150ms) {
    if (currentSw1 && !prevSw1) {
      sw1Pressed = true;
      debounceTimer.reset();
    }

    if (currentSw2 && !prevSw2) {
      sw2Pressed = true;
      debounceTimer.reset();
    }
  }
  prevSw1 = currentSw1;
  prevSw2 = currentSw2;
}

bool isSw1Pressed() { return sw1Pressed; }

bool isSw2Pressed() { return sw2Pressed; }
