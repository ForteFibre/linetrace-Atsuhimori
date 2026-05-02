#pragma once

#include <drivers/PwmOut.h>

#include <algorithm>
#include <cmath>

class Motor
{
private:
  mbed::PwmOut _a, _b;
  bool _reverse_logic;
  float _duty;

  static constexpr auto Freq = 5000;
  static constexpr auto DutyAbsMax = 0.97f;

public:
  explicit Motor(PinName a, PinName b, bool reverse_logic)
  : _a(a), _b(b), _reverse_logic(reverse_logic), _duty(0)
  {
    _a.period_us(1e6 / Freq);
    _b.period_us(1e6 / Freq);
  }

  void reset() {}

  void set_output(float duty)
  {
    _duty = duty;
    update();
  }

  auto output() -> float { return _duty; }

  void update()
  {
    auto speed = std::clamp(_duty, -DutyAbsMax, DutyAbsMax);

    // 論理が逆の場合，出力が逆になるように．
    if (_reverse_logic) {
      if (std::signbit(speed)) {
        speed = -1.f - speed;
      } else {
        speed = 1.f - speed;
      }
    }

    if (std::signbit(speed)) {
      _a.write(-speed);  // 負の場合正に
      _b.write(0.f);
    } else {
      _a.write(0.f);
      _b.write(speed);
    }
  }

  void operator=(float duty) { set_output(duty); }
};