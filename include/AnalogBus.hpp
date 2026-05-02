#pragma once

#include <mbed.h>

#include <initializer_list>
#include <memory>
#include <vector>

class AnalogBus
{
private:
  std::vector<std::unique_ptr<AnalogIn>> analogs;

public:
  AnalogBus(std::initializer_list<PinName> pins)
  {
    for (auto pin : pins) {
      analogs.push_back(std::make_unique<AnalogIn>(pin));
    }
  }

  uint16_t read_u16(size_t index) { return analogs.at(index)->read_u16(); }

  float read(size_t index) { return analogs.at(index)->read(); }
};