#ifndef FORTEFIBRE_ENCODER_H
#define FORTEFIBRE_ENCODER_H

#include <cmath>

#include "hal/encoder_api.h"
#include "platform/CriticalSectionLock.h"

class Encoder
{
public:
  Encoder(PinName pin1, PinName pin2, float gain = 1.0)
  : _pin1(pin1),
    _pin2(pin2),
    _gain(gain),
    _initialized(false),
    _prev_counter(0),
    _speed(0),
    _position(0)
  {
    start();
  }

  ~Encoder() { deinit(); }

  void start()
  {
    mbed::CriticalSectionLock lock;
    if (!_initialized) {
      init();
      _prev_counter = read();
    }
  }

  void stop()
  {
    mbed::CriticalSectionLock lock;
    if (_initialized) {
      deinit();
    }
  }

  void reset()
  {
    mbed::CriticalSectionLock lock;
    _position = 0;
    _speed = 0;
    _prev_counter = 0;
  }

  count_t read()
  {
    mbed::CriticalSectionLock lock;
    count_t ret = 0;
    if (_initialized) {
      ret = encoder_read(&_encoder);
    }
    return ret;
  }

  float speed() const noexcept { return _gain * raw_speed(); }

  float position() const noexcept { return _gain * raw_position(); }

  void set_position(float position) { _position = position / _gain; }

  void reset_position_offset(float offset) { _position = (position() - offset) / _gain; }

  int64_t raw_speed() const noexcept { return _speed; }

  int64_t raw_position() const noexcept { return _position; }

  void update()
  {
    mbed::CriticalSectionLock lock;

    if (!_initialized) return;

    count_t currnet_counter = read();
    int64_t diff = (int64_t)currnet_counter - _prev_counter;
    _prev_counter = currnet_counter;
    if (abs(diff) > (0xFFFF >> 1)) {
      _speed = diff - copysign(0xFFFF, diff);
    } else {
      _speed = diff;
    }

    _position += _speed;
  }

  void set_gain(float gain) { _gain = gain; }

  float get_gain() const { return _gain; }

private:
  void init()
  {
    mbed::CriticalSectionLock lock;
    if (!_initialized) {
      encoder_init(&_encoder, _pin1, _pin2);
      _initialized = true;
    }
  }

  void deinit()
  {
    mbed::CriticalSectionLock lock;
    if (_initialized) {
      encoder_free(&_encoder);
      _initialized = false;
    }
  }

  PinName _pin1, _pin2;
  encoder_t _encoder;
  float _gain;
  bool _initialized;
  count_t _prev_counter;
  int64_t _speed;
  int64_t _position;
};

#endif
