#ifndef FORTEFIBRE_ENCODER_API_H
#define FORTEFIBRE_ENCODER_API_H

#include <stdint.h>

#include "PinNames.h"
#include "pinmap.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t count_t;

typedef struct
{
  TIM_HandleTypeDef TimHandle;
  PinName ch1;
  PinName ch2;
} encoder_t;

typedef struct
{
  uint32_t peripheral;
  PinName ch1_pin;
  uint32_t ch1_function;
  PinName ch2_pin;
  uint32_t ch2_function;
} encoder_pinmap_t;

void encoder_init_direct(encoder_t * obj, const encoder_pinmap_t * pinmap);

void encoder_init(encoder_t * obj, PinName input1, PinName input2);

void encoder_free(encoder_t * obj);

count_t encoder_read(encoder_t * obj);

#ifdef __cplusplus
}
#endif

#endif
