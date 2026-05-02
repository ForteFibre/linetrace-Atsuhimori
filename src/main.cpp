
#include <mbed.h>

#include <AnalogBus.hpp>
#include <Encoder.hpp>
#include <Motor.hpp>
#include <PIDController.hpp>
#include <cstdio>

int main()
{
  Motor motor_0(PB_8, PB_9, false);
  Motor motor_1(PB_10, PB_2, false);
  AnalogBus sensor({PC_0, PC_1, PB_0, PA_4_ALT0, PA_1, PA_0});
  DigitalOut led_a(PC_11), led_b(PC_10), led_c(PC_12), ld2(LED1 /* Nucleo */);
  DigitalIn sw1(PA_15), sw2(PC_2), sw3(PC_3), b1(BUTTON1 /* Nucleo */);
  Encoder encoder_0(PB_4, PB_5, 1.0);
  Encoder encoder_1(PA_8, PA_9, 1.0);

  while (true) {
    motor_0.set_output(-1.0);
    motor_1.set_output(-1.0);
    ThisThread::sleep_for(100ms);
    printf(
      "Sensor: %f %f %f %f %f %f\n", sensor.read(0), sensor.read(1), sensor.read(2), sensor.read(3),
      sensor.read(4), sensor.read(5));
    encoder_0.update();
    encoder_1.update();
    printf("Encoder: %f %f\n", encoder_0.position(), encoder_1.position());
    led_a = !sw1;
    led_b = !sw2;
    led_c = !sw3;
    ld2 = !b1;
  }
}