//mbedのThisThreadはたまにばぐる
#include <mbed.h>

#include <AnalogBus.hpp>
#include <Encoder.hpp>
#include <Motor.hpp>

//#include <PIDController.hpp>
//#include <cmath>
//#include <cstdio>

#include "button.h"
#include "lineTrace.h"
#include "motor.h"
#include "sensor.h"
#include "state.h"

//int sensor_max[5] = {0, 0, 0, 0, 0};
//int sensor_min[5] = {0, 0, 0, 0, 0};

Motor motor_0(PB_8, PB_9, false);
Motor motor_1(PB_10, PB_2, false);

AnalogBus sensor({PC_0, PC_1, PB_0, PA_4_ALT0, PA_1, PA_0});

DigitalIn sw1(PA_15), sw2(PC_2), sw3(PC_3), b1(BUTTON1 /* Nucleo */);

DigitalOut led_a(PC_11), led_b(PC_10), led_c(PC_12), ld2(LED1 /* Nucleo */);

//Encoder encoder_0(PB_4, PB_5, 1.0);
//Encoder encoder_1(PA_8, PA_9, 1.0);

int main()
{
  while (true) {
    buttonUpdate();

    switch (robotState) {
      case STOP:

        led_a = 1;
        led_b = 0;
        led_c = 0;

        motorStop();

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

        /* for (int i = 0; i < 6; i++) {
          printf("%.2f", readNormalized(i));
        }
        printf("\n");
        ThisThread::sleep_for(100ms);
        値を読む用
        */

        sensorUpdate();

        lineTraceUpdate();

        if (isSw1Pressed()) {
          motorStop();

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

    ThisThread::sleep_for(100ms);
  }

  /*while (true) {
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
  }*/
  /*
  while (true) {
    printf("ad[0]: %d ", sensor.read_u16(0));  //右から順に0,1,2,3,4,5
    printf("ad[1]: %d ", sensor.read_u16(1));
    printf("ad[2]: %d ", sensor.read_u16(2));
    printf("ad[3]: %d ", sensor.read_u16(3));
    printf("ad[4]: %d ", sensor.read_u16(4));
    printf("ad[5]: %d ", sensor.read_u16(5));
    printf("\n");  // 最後に改行して見やすくする

    // 0.1秒止める
    ThisThread::sleep_for(100ms);
  }
*/
  //ThisThread::sleep_for(100ms);
  //Kernel::Clock::now();

  /*led_a.write(true);
  ThisThread::sleep_for(1000ms);
  led_a.write(false);
*/
  /*
  while (true) {
    if (sw1.read() == false) {
      led_a.write(true);
      ThisThread::sleep_for(10ms);
    }

    else {
      led_a.write(false);
      ThisThread::sleep_for(10ms);
    }
  }
*/
  /*
while (true) {
  //直進
  motor_0.set_output(-0.7);  //左モーター
  motor_1.set_output(-0.7);  //→モーター
  ThisThread::sleep_for(3000ms);
  motor_0.set_output(0);
  motor_1.set_output(0);
  ThisThread::sleep_for(2000ms);
  //右回転
  motor_0.set_output(-0.9);   //左モーター
  motor_1.set_output(-0.85);  //→モーター
  ThisThread::sleep_for(2000ms);
  motor_0.set_output(0);
  motor_1.set_output(0);
  ThisThread::sleep_for(2000ms);
  //その場回転
  motor_0.set_output(0.8);   //左モーター
  motor_1.set_output(-0.8);  //→モーター
  ThisThread::sleep_for(2000ms);
  motor_0.set_output(0);
  motor_1.set_output(0);
  ThisThread::sleep_for(2000ms);
}
*/
  /*
while (true) {

  if (sensor.read_u16(0) < 30000){
    printf("ad[0]: 0, " );//右から順に0,1,2,3,4,5
  }
  else {
    printf("ad[0]: X, " );//右から順に0,1,2,3,4,5
  }
    if (sensor.read_u16(1) < 40000){
    printf("ad[1]: 0,  " );//右から順に0,1,2,3,4,5
  }
  else {
    printf("ad[1]: X, " );//右から順に0,1,2,3,4,5
  }
    if (sensor.read_u16(2) < 10000){
    printf("ad[2]: 0, " );//右から順に0,1,2,3,4,5
  }
  else {
    printf("ad[2]: X, " );//右から順に0,1,2,3,4,5
  }
    if (sensor.read_u16(3) < 30000){
    printf("ad[3]: 0," );//右から順に0,1,2,3,4,5
  }
  else {
    printf("ad[3]: X, " );//右から順に0,1,2,3,4,5
  }
    if (sensor.read_u16(4) < 20000){
    printf("ad[4]: 0, " );//右から順に0,1,2,3,4,5
  }
  else {
    printf("ad[4]: X, " );//右から順に0,1,2,3,4,5
  }  
    if (sensor.read_u16(5) < 25000){
    printf("ad[5]: 0, ");//右から順に0,1,2,3,4,5
  }
  else {
    printf("ad[5]: X, " );//右から順に0,1,2,3,4,5
  }

    printf("\n");  // 最後に改行して見やすくする

    // 0.1秒止める
    ThisThread::sleep_for(100ms);
  }
*/

  /*
encoder_0.update();
encoder_1.update();

float speed_0 = encoder_0.speed();
float speed_1 = encoder_1.speed();

*/
  /*
while (true) {
  encoder_0.update(); 
  encoder_1.update();              // 回転速度を更新
  motor_0.set_output(-0.7);  //左モーター
  motor_1.set_output(-0.7);  //→モーター
  ThisThread::sleep_for(100ms);
  float speed_0 = encoder_0.speed(); 
  float speed_1 = encoder_1.speed();   // 回転速度を取得
  printf("現在の速度：%f %f\n", speed_0, speed_1); // 得た回転速度を出力
  ThisThread::sleep_for(100ms);     // 0.1秒待つ
  //300ぐらいで回転速度走らせる
  //最初は200でいいかも
}
  */
  /*
// センサの最大値を格納する配列。初期値はすべて0
int sensor_max[6] = {0, 0, 0, 0, 0, 0};
// センサの最小値を格納する配列。初期値はセンサでとれる値の範囲が0~65535なので65535
int sensor_min[6] = {65535, 65535, 65535, 65535, 65535, 65535};

// 100 回、センサで値をとる
for(int i = 0; i < 100; i++) {
    ThisThread::sleep_for(100ms); // 0.1 秒待つ
    // センサーでとった値を配列に入れる
    int sensor_now[6] = {
        sensor.read_u16(0),
        sensor.read_u16(1),
        sensor.read_u16(2),
        sensor.read_u16(3),
        sensor.read_u16(4),
        sensor.read_u16(5),
    };

    // センサの数だけfor分を回す
    for(int j = 0; j < 6; j++){
        // 今までの最大値とこれまでの値を比較して最大値・最小値を更新する
        // std::max(a, b) は a と b のうち大きい方を返す関数です
        // std::min(a, b) は a と b のうち小さい方を返す関数です
        sensor_max[j] = std::max(sensor_max[j], sensor_now[j]);
        sensor_min[j] = std::min(sensor_min[j], sensor_now[j]);
    }
    // この時点でsensor_max[i]にはこれまでのデータの最大値
    // sensor_min[i]にはこれまでのデータの最小値が入っている。
}

// とった最大・最小値を表示してみる。
printf(" |  min \t max\n");
for(int i = 0; i < 4; i++){
    printf("sensor[%d] | %d \t %d\n", i, sensor_min[i], sensor_max[i]);
}

ThisThread::sleep_for(30000ms);


*/

  /*
while (true) {
int threshold[6] = {100, 105, 98, 130, 104, 99};  //　それぞれのセンサに対応した閾値

    if (adc.read_u16(0) > threshold[0]) { // 閾値より大きければ...
    } 
    else {
    }
*/
}