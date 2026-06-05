//mbedのThisThreadはたまにばぐる
#include <mbed.h>

#include <AnalogBus.hpp>
#include <Encoder.hpp>
#include <Motor.hpp>

#include "button.h"
#include "lineTrace.h"
#include "motor.h"
#include "sensor.h"
#include "setting.h"
#include "speedControl.h"
#include "speedMode.h"
#include "speedTest.h"
#include "state.h"

Motor motor_0(PB_8, PB_9, false);
Motor motor_1(PB_10, PB_2, false);

AnalogBus sensor({PC_0, PC_1, PB_0, PA_4_ALT0, PA_1, PA_0});

DigitalIn sw1(PA_15), sw2(PC_2), sw3(PC_3), b1(BUTTON1);

DigitalOut led_a(PC_11), led_b(PC_10), led_c(PC_12), ld2(LED1);

// =========================
// Encoder
// =========================

Encoder encoder_0(PB_4, PB_5, 1.0);
Encoder encoder_1(PA_8, PA_9, 1.0);

// =========================
// 制御周期
// =========================

static const auto CONTROL_PERIOD = 5ms;

int main()
{
  // =========================
  // SPEED_CONTROL設定
  // =========================

  setBaseSpeed(BASE_SPEED);

  setLineTracePID(SPEED_KP, SPEED_KI, SPEED_KD);

  // =========================
  // DIRECT_PWM設定
  // =========================

  setBasePwm(BASE_PWM);

  setDirectPwmPID(PWM_KP, PWM_KI, PWM_KD);

  while (true) {
    buttonUpdate();

    switch (robotState) {
      case STOP:

        led_a = 1;
        led_b = 0;
        led_c = 0;

        resetSpeedControl();

        if (isSw1Pressed()) {
          robotState = RUN;
        }

        if (isSw2Pressed()) {
          calibrationStart();
          robotState = CALIBRATION;
        }

        break;

      case RUN:

        led_a = 0;
        led_b = 1;
        led_c = 0;

        sensorUpdate();

        lineTraceUpdate();

        if (driveMode == SPEED_CONTROL) {
          speedControlUpdate();
        }

        if (isSw1Pressed()) {
          resetSpeedControl();

          resetLineTrace();

          robotState = STOP;
        }

        break;

      case CALIBRATION:

        led_a = 0;
        led_b = 0;
        led_c = 1;

        calibrationUpdate();

        if (calibrationFinished()) {
          robotState = STOP;
        }

        break;
    }

    ThisThread::sleep_for(CONTROL_PERIOD);
  }
}