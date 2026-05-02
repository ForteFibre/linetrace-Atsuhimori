// clang-format off
#include "PeripheralPins.h"
#include "PinNames.h"
#include "mbed_toolchain.h"
#include "stm32f446xx.h"
#include "stm32f4xx_hal_gpio_ex.h"

typedef enum {
  ENC_0 = TIM3_BASE,
  ENC_1 = TIM1_BASE,
} EncoderName;

const PinMap PinMap_Encoder_TIM[] = {
  {PB_4, ENC_0, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 1, 0)},
  {PB_5, ENC_0, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 2, 0)},
  {PA_8, ENC_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 1, 0)},
  {PA_9, ENC_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 2, 0)},
  {NC, NC, 0},
};

// clang-format on