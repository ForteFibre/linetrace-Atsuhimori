#include "button.h"

#include <mbed.h>

extern DigitalIn sw1;
extern DigitalIn sw2;

static bool prevSw1 = false;
static bool prevSw2 = false;

static bool sw1Pressed = false;
static bool sw2Pressed = false;

void buttonUpdate()
{
  bool currentSw1 = !sw1;
  bool currentSw2 = !sw2;

  if (currentSw1 && !prevSw1) {
    sw1Pressed = true;
  }

  if (currentSw2 && !prevSw2) {
    sw2Pressed = true;
  }

  prevSw1 = currentSw1;
  prevSw2 = currentSw2;
}

bool isSw1Pressed() { return sw1Pressed; }

bool isSw2Pressed() { return sw2Pressed; }
