#include "speedMode.h"

#include "setting.h"

#if DRIVE_MODE_SPEED_CONTROL

DriveMode driveMode = SPEED_CONTROL;

#else

DriveMode driveMode = DIRECT_PWM;

#endif